#include <cassert>
#include <iostream>
#include <fstream>
#include <functional>
#include <thread>
#include <algorithm>
#include <iomanip>
#include "Utils.h"

namespace DNest4
{

template<class ModelType>
Sampler<ModelType>::Sampler(unsigned int num_threads, double compression,
							const Options& options, bool save_to_disk,
                            bool _adaptive)
:save_to_disk(save_to_disk)
,thin_print(1)
,threads(num_threads, nullptr)
,barrier(nullptr)
,num_threads(num_threads)
,compression(compression)
,options(options)
,adaptive(_adaptive)
,particles(options.num_particles*num_threads)
,log_likelihoods(options.num_particles*num_threads)
,level_assignments(options.num_particles*num_threads, 0)
,levels(1, LikelihoodType())
,copies_of_levels(num_threads, levels)
,all_above()
,rngs(num_threads)
,count_saves(0)
,count_mcmc_steps_since_save(0)
,count_mcmc_steps(0)
,difficulty(1.0)
,work_ratio(1.0)
,above(num_threads)
{
	assert(num_threads >= 1);
	assert(compression > 1.);

    if(options.max_num_levels == 0 && std::abs(compression - exp(1.0)) > 1E-6)
    {
        std::cerr<<"# ERROR: Cannot use -c with max_num_levels=0 (AUTO).";
        std::cerr<<std::endl;
        exit(0);
    }

    // Find best ever particle
    auto indices = argsort(log_likelihoods);
    best_ever_particle = particles[indices.back()];
    best_ever_log_likelihood = log_likelihoods[indices.back()];
}

template<class ModelType>
void Sampler<ModelType>::initialise(unsigned int first_seed)
{
	// Reference to an RNG to use
	RNG& rng = rngs[0];

	// Assign memory for storage
	all_above.reserve(2*options.new_level_interval);
	for(auto& a: above)
		a.reserve(2*options.new_level_interval);

	std::cout<<"# Seeding random number generators. First seed = ";
	std::cout<<first_seed<<"."<<std::endl;
	// Seed the RNGs, incrementing the seed each time
	for(RNG& rng: rngs)
		rng.set_seed(first_seed++);

	std::cout<<"# Generating "<<particles.size();
	std::cout<<" particle"<<((particles.size() > 1)?("s"):(""));
	std::cout<<" from the prior..."<<std::flush;
	for(size_t i=0; i<particles.size(); ++i)
	{
		particles[i].from_prior(rng);
		log_likelihoods[i] = LikelihoodType(particles[i].log_likelihood(),
																rng.rand());
	}
	std::cout<<"done."<<std::endl;
	initialise_output_files();
}

template<class ModelType>
void Sampler<ModelType>::run(unsigned int thin)
{
	// Set the thining of terminal output
	thin_print = thin;

#ifndef NO_THREADS
	// Set up threads and barrier
	// Delete if necessary (shouldn't be needed!)
	if(barrier != nullptr)
		delete barrier;
	for(auto& thread: threads)
	{
		if(thread != nullptr)
			delete thread;
	}

	// Create the barrier
	barrier = new Barrier(num_threads);

	// Create and launch threads
	for(size_t i=0; i<threads.size(); ++i)
	{
		// Function to run on each thread
		auto func = std::bind(&Sampler<ModelType>::run_thread, this, i);

		// Allocate and create the thread
		threads[i] = new std::thread(func);
	}

	// Join and de-allocate all threads and barrier
	for(auto& t: threads)
		t->join();

	// Delete dynamically allocated stuff and point to nullptr
	delete barrier;
	barrier = nullptr;
	for(auto& t: threads)
	{
		delete t;
		t = nullptr;
	}
#else
	for(size_t i=0; i<threads.size(); ++i) run_thread(i);
#endif

	// Save the sampler state to a file.
	std::fstream fout("sampler_state.txt", std::ios::out);
	print(fout);
	fout.close();
}

template<class ModelType>
void Sampler<ModelType>::mcmc_thread(unsigned int thread)
{
	// Reference to the RNG for this thread
	RNG& rng = rngs[thread];

	// Reference to this thread's copy of levels
	std::vector<Level>& _levels = copies_of_levels[thread];

	// First particle belonging to this thread
	const int start_index = thread*options.num_particles;

	// Do some MCMC
	int which;
	for(unsigned int i=0; i<options.thread_steps; ++i)
	{
		which = start_index + rng.rand_int(options.num_particles);

		if(rng.rand() <= 0.5)
		{
			update_particle(thread, which);
			update_level_assignment(thread, which);
		}
		else
		{
			update_level_assignment(thread, which);
			update_particle(thread, which);
		}
		if(!enough_levels(_levels) &&
				_levels.back().get_log_likelihood() < log_likelihoods[which])
			above[thread].push_back(log_likelihoods[which]);
	}
}

template<class ModelType>
void Sampler<ModelType>::update_particle(unsigned int thread, unsigned int which)
{
	// Reference to the RNG for this thread
	RNG& rng = rngs[thread];

	// Reference to copies_of_levels[thread]
	std::vector<Level>& _levels = copies_of_levels[thread];

	// Reference to the level we're in
	Level& level = _levels[level_assignments[which]];

	// Reference to the particle being moved
	ModelType& particle = particles[which];
	LikelihoodType& logl = log_likelihoods[which];

	// Do the proposal for the particle
	ModelType proposal = particle;
	double log_H = proposal.perturb(rng);

	// Prevent unnecessary exponentiation of a large number
	if(log_H > 0.0)
		log_H = 0.0;

    if(rng.rand() <= exp(log_H))
    {
    	LikelihoodType logl_proposal(proposal.log_likelihood(),
												logl.get_tiebreaker());

    	// Do the proposal for the tiebreaker
    	log_H += logl_proposal.perturb(rng);

	    // Accept?
	    if(level.get_log_likelihood() < logl_proposal)
	    {
		    particle = proposal;
		    logl = logl_proposal;
		    level.increment_accepts(1);
	    }
    }

	level.increment_tries(1);

	// Count visits and exceeds
	unsigned int current_level = level_assignments[which];
	for(; current_level < (_levels.size()-1); ++current_level)
	{
		_levels[current_level].increment_visits(1);
		if(_levels[current_level+1].get_log_likelihood() <
											log_likelihoods[which])
			_levels[current_level].increment_exceeds(1);
		else
			break;
	}
}

template<class ModelType>
void Sampler<ModelType>::update_level_assignment(unsigned int thread,
													unsigned int which)
{
	// Reference to the RNG for this thread
	RNG& rng = rngs[thread];

	// Reference to this thread's copy of levels
	std::vector<Level>& _levels = copies_of_levels[thread];

	// Generate proposal
	int proposal = static_cast<int>(level_assignments[which])
						+ static_cast<int>(pow(10., 2.*rng.rand())*rng.randn());

	// If the proposal was to not move, go +- one level
	if(proposal == static_cast<int>(level_assignments[which]))
		proposal = ((rng.rand() < 0.5)?(proposal-1):(proposal+1));

	// Wrap into allowed range
	proposal = DNest4::mod(proposal, static_cast<int>(_levels.size()));

	// Acceptance probability
	double log_A = -_levels[proposal].get_log_X()
					+ _levels[level_assignments[which]].get_log_X();

	// Pushing up part
	log_A += log_push(proposal) - log_push(level_assignments[which]);

	// Enforce uniform exploration part (if all levels exist)
	if(_levels.size() == options.max_num_levels)
		log_A += options.beta*log((double)(_levels[level_assignments[which]].get_tries() + 1)/(double)(_levels[proposal].get_tries() + 1));

	// Prevent exponentiation of huge numbers
	if(log_A > 0.)
		log_A = 0.;

	// Make a LikelihoodType for the proposal
	if(rng.rand() <= exp(log_A) && _levels[proposal].get_log_likelihood() < log_likelihoods[which])
	{
		// Accept
		level_assignments[which] = static_cast<unsigned int>(proposal);
	}
}

template<class ModelType>
void Sampler<ModelType>::run_thread(unsigned int thread)
{
	// Alternate between MCMC and bookkeeping
	while(true)
	{
		// Thread zero takes full responsibility for some tasks
		// Setting up copies of levels
		if(thread == 0)
		{
			// Each thread will write over its own copy of the levels
			for(unsigned int i=0; i<num_threads; ++i)
				copies_of_levels[i] = levels;
		}

#ifndef NO_THREADS
		// Wait for all threads to get here before proceeding
		barrier->wait();
#endif

		// Check for termination
		if(options.max_num_saves != 0 && count_saves != 0 &&
				(count_saves%options.max_num_saves == 0))
			return;

		// Do the MCMC (all threads do this!)
		mcmc_thread(thread);

#ifndef NO_THREADS
		barrier->wait();
#endif

		// Thread zero takes full responsibility for some tasks
		if(thread == 0)
		{
			// Count the MCMC steps done
			count_mcmc_steps += num_threads*options.thread_steps;
            count_mcmc_steps_since_save += num_threads*options.thread_steps;

			// Go through copies of levels and apply diffs to levels
			std::vector<Level> levels_orig = levels;
			for(const auto& _levels: copies_of_levels)
			{
				for(size_t i=0; i<levels.size(); ++i)
				{
					levels[i].increment_accepts(_levels[i].get_accepts()
														- levels_orig[i].get_accepts());
					levels[i].increment_tries(_levels[i].get_tries()
														- levels_orig[i].get_tries());
					levels[i].increment_visits(_levels[i].get_visits()
														- levels_orig[i].get_visits());
					levels[i].increment_exceeds(_levels[i].get_exceeds()
														- levels_orig[i].get_exceeds());
				}
			}

			// Combine into a single vector
			for(auto& a: above)
			{
				for(const auto& element: a)
					all_above.push_back(element);
				a.clear();
			}

			// Do the bookkeeping
			do_bookkeeping();
		}
	}
}

template<class ModelType>
void Sampler<ModelType>::increase_max_num_saves(unsigned int increment)
{
	options.max_num_saves += increment;
}

template<class ModelType>
bool Sampler<ModelType>::enough_levels(const std::vector<Level>& l) const
{
    if(options.max_num_levels == 0)
    {
        // Check level spacing (in terms of log likelihood)
        // over last n levels
        int num_levels_to_check = static_cast<int>(30*sqrt(0.02*l.size()));
        if(num_levels_to_check < 30)
            return false;

        int k = l.size() - 1;
        double tot = 0.0;
        double max = -1E300;
        for(int i=0; i<num_levels_to_check; ++i)
        {
            double diff = l[k].get_log_likelihood().get_value()
                             - l[k-1].get_log_likelihood().get_value();
            tot += diff;
            if(diff > max)
                max = diff;
            --k;
        }
        if(tot / num_levels_to_check < 0.75 && max < 1.0)
            return true;
        else
            return false;
    }

    // Just compare with the value from OPTIONS
    return (l.size() >= options.max_num_levels);   
}

template<class ModelType>
void Sampler<ModelType>::do_bookkeeping()
{
	bool created_level = false;

	// Create a new level?
	if(!enough_levels(levels) &&
        (all_above.size() >= options.new_level_interval))
	{
		// Create the level
		std::sort(all_above.begin(), all_above.end());
		int index = static_cast<int>((1. - 1./compression)*all_above.size());
		std::cout<<"# Creating level "<<levels.size()<<" with log likelihood = ";
		std::cout<<std::setprecision(12);
		std::cout<<all_above[index].get_value()<<"."<<std::endl;

		levels.push_back(Level(all_above[index]));
		all_above.erase(all_above.begin(), all_above.begin() + index + 1);
		for(auto& a:above)
			a.clear();

		// If last level
		if(enough_levels(levels))
		{
            // Regularisation
            double reg = options.new_level_interval*sqrt(options.lambda);
			Level::renormalise_visits(levels, static_cast<int>(reg));
			all_above.clear();
            options.max_num_levels = levels.size();
            std::cout<<"# Done creating levels."<<std::endl;
		}
		else
		{
			// If it's not the last level, look for lagging particles
			kill_lagging_particles();
		}

		created_level = true;
	}

	// Recalculate log_X values of levels
	Level::recalculate_log_X(levels, compression,
                        options.new_level_interval*sqrt(options.lambda));


    if(!enough_levels(levels) && adaptive)
    {
        // Compute difficulty as a weighted average of compression deviations
        // from the target value
        double gap_norm_tot = 0.0;
        double weight_tot = 0.0;
        for(size_t i=1; i<levels.size(); ++i)
        {
            // Departure of log(X) differences from target value.
            double gap = (levels[i-1].get_log_X() - levels[i].get_log_X())
                            - log(compression);
            double weight = exp(((int)i - (int)levels.size())/3.0);
            gap_norm_tot += weight*std::abs(gap)/log(compression);
            weight_tot += weight;
        }
        difficulty = gap_norm_tot / weight_tot;

        double work_ratio_max = 20.0/sqrt(options.lambda);
        double coeff = (work_ratio_max - 1.0)/(0.1 - 0.02); 
        if(difficulty >= 0.1)
            work_ratio = work_ratio_max;
        else if(difficulty >= 0.02)
            work_ratio = 1.0 + coeff*(difficulty - 0.02);
        else
            work_ratio = 1.0;
    }

	// Save levels if one was created
	if(created_level)
		save_levels();

	if(count_mcmc_steps_since_save >= options.save_interval)
	{
		save_particle();

		// If a level was not created, save anyway because of the time
		if(!created_level)
			save_levels();


        // Print work ratio
        if(!enough_levels(levels) && adaptive)
        {
            std::cout << "# Difficulty = " << difficulty << ".\n";
            std::cout << "# Work ratio = " << work_ratio << ".\n" << std::endl;
        }
	}

    // Check for a new record holder
    auto indices = argsort(log_likelihoods);
    if(best_ever_log_likelihood < log_likelihoods[indices.back()])
    {
        best_ever_particle = particles[indices.back()];
        best_ever_log_likelihood = log_likelihoods[indices.back()];
    }
}

template<class ModelType>
double Sampler<ModelType>::log_push(unsigned int which_level) const
{
	// Reference to this thread's copy of levels
	assert(which_level < levels.size());
	if(enough_levels(levels))
		return 0.0;

	int i = which_level - (static_cast<int>(levels.size()) - 1);
	return static_cast<double>(i)/(work_ratio*options.lambda);
}

template<class ModelType>
void Sampler<ModelType>::initialise_output_files() const
{
	if(!save_to_disk)
		return;

	std::fstream fout;

	// Output headers
	fout.open(options.sample_info_file, std::ios::out);
	fout<<"# level assignment, log likelihood, tiebreaker, ID."<<std::endl;
	fout.close();

	fout.open(options.sample_file, std::ios::out);
	fout<<"# "<<particles[0].description().c_str()<<std::endl;
	fout.close();

	save_levels();
}


template<class ModelType>
void Sampler<ModelType>::save_levels() const
{
	if(!save_to_disk)
		return;

	// Output file
	std::fstream fout;
	fout.open(options.levels_file, std::ios::out);
	fout<<"# log_X, log_likelihood, tiebreaker, accepts, tries, exceeds, visits";
	fout<<std::endl;
    fout<<std::setprecision(12);


	for(const Level& level: levels)
	{
		fout<<level.get_log_X()<<' ';
		fout<<level.get_log_likelihood().get_value()<<' ';
		fout<<level.get_log_likelihood().get_tiebreaker()<<' ';
		fout<<level.get_accepts()<<' ';
		fout<<level.get_tries()<<' ';
		fout<<level.get_exceeds()<<' ';
		fout<<level.get_visits()<<std::endl;
	}
	fout.close();
}

template<class ModelType>
void Sampler<ModelType>::save_particle()
{
	++count_saves;
    count_mcmc_steps_since_save = 0;

	if(!save_to_disk)
		return;

	if(count_saves % thin_print == 0)
		std::cout<<"# Saving particle to disk. N = "<<count_saves<<"."<<std::endl;

	// Output file
	std::fstream fout;

	int which = rngs[0].rand_int(particles.size());
	fout.open(options.sample_file, std::ios::out|std::ios::app);
	particles[which].print(fout);
	fout<<std::endl;
	fout.close();

	fout.open(options.sample_info_file, std::ios::out|std::ios::app);
	fout<<std::setprecision(12);

	fout<<level_assignments[which]<<' ';
	fout<<log_likelihoods[which].get_value()<<' ';
	fout<<log_likelihoods[which].get_tiebreaker()<<' ';
	fout<<which<<std::endl;
	fout.close();
}

template<class ModelType>
void Sampler<ModelType>::kill_lagging_particles()
{
	// Count the number of deletions
	static unsigned int deletions = 0;

	// Flag each particle as good or bad
	std::vector<bool> good(num_threads*options.num_particles, true);

	// How good is the best particle?
	double max_log_push = -std::numeric_limits<double>::max();

	unsigned int num_bad = 0;
    double kill_probability = 0.0;
	for(size_t i=0; i<particles.size(); ++i)
	{
		if(log_push(level_assignments[i]) > max_log_push)
			max_log_push = log_push(level_assignments[i]);
        kill_probability = pow(1.0 - 1.0/(1.0 + exp(-log_push(level_assignments[i]) - 4.0)), 3);

		if(rngs[0].rand() <= kill_probability)
		{
			good[i] = false;
			++num_bad;
		}
	}

	if(num_bad < num_threads*options.num_particles)
	{
		// Replace bad particles with copies of good ones
		for(size_t i=0; i<particles.size(); ++i)
		{
			if(!good[i])
			{
				// Choose a replacement particle. Higher prob
				// of selecting better particles.
				int i_copy;
				do
				{
					i_copy = rngs[0].rand_int(num_threads*options.num_particles);
				}while(!good[i_copy] ||
			        rngs[0].rand() >= exp(log_push(level_assignments[i_copy]) - max_log_push));

				particles[i] = particles[i_copy];
				log_likelihoods[i] = log_likelihoods[i_copy];
				level_assignments[i] = level_assignments[i_copy];
				++deletions;

				std::cout<<"# Replacing lagging particle.";
				std::cout<<" This has happened "<<deletions;
				std::cout<<" times."<<std::endl;
			}
		}
	}
}

template<class ModelType>
void Sampler<ModelType>::print(std::ostream& out) const
{
	out<<save_to_disk<<' ';
	out<<num_threads<<' ';
	out<<compression<<' ';

	out<<options<<' ';

	for(const auto& p: particles)
		p.print(out);

	for(const auto& l: log_likelihoods)
		l.print(out);

	for(const auto& l: level_assignments)
		out<<l<<' ';

	for(const auto& l: levels)
		l.print(out);

	out<<count_saves<<' ';
	out<<count_mcmc_steps<<' ';
}

template<class ModelType>
void Sampler<ModelType>::read(std::istream& in)
{
	in>>save_to_disk;
	in>>num_threads;
	in>>compression;

	in>>options;

	for(auto& p: particles)
		p.read(in);

	for(auto& l: log_likelihoods)
		l.read(in);

	for(auto l: level_assignments)
		in>>l;

	for(auto& l: levels)
		l.read(in);

	in>>count_saves;
	in>>count_mcmc_steps;
}

} // namespace DNest4

