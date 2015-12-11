#include <cassert>
#include <iostream>
#include <thread>
#include "Utils.h"

namespace DNest4
{

template<class ModelType>
Sampler<ModelType>::Sampler(unsigned int num_threads, double compression,
							const Options& options)
:num_threads(num_threads)
,compression(compression)
,options(options)
,particles(options.num_particles*num_threads)
,log_likelihoods(options.num_particles*num_threads)
,level_assignments(options.num_particles*num_threads, 0)
,levels(1, LikelihoodType())
,copies_of_levels(num_threads, levels)
,rngs(num_threads)
,log_likelihood_keep()
,saves(0)
{
	assert(num_threads >= 1);
	assert(compression > 1.);
}

template<class ModelType>
void Sampler<ModelType>::initialise(unsigned int first_seed)
{
	// Reference to an RNG to use
	RNG& rng = rngs[0];

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
	std::cout<<"done."<<std::endl<<std::endl;
}

template<class ModelType>
void Sampler<ModelType>::do_mcmc_thread(unsigned int thread)
{
	// Reference to the RNG for this thread
	RNG& rng = rngs[thread];

	// First particle belonging to this thread
	const int start_index = thread*options.num_particles;

	// Do some MCMC
	int which;
	for(unsigned int i=0; i<options.thread_steps; ++i)
	{
		which = start_index + rng.rand_int(options.num_particles);
		update_particle(thread, which);
		update_level_assignment(thread, which);
	}
}

template<class ModelType>
void Sampler<ModelType>::do_mcmc()
{
	// Each thread will write over its own copy of the levels
	for(unsigned int i=0; i<num_threads; ++i)
		copies_of_levels[i] = levels;

	// Create the threads
	std::vector<std::thread> threads;
	for(unsigned int i=0; i<num_threads; ++i)
	{
		auto func = std::bind(&Sampler<ModelType>::do_mcmc_thread, this, i);
		threads.push_back(std::thread(func));
	}
	for(std::thread& t: threads)
		t.join();

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
	LikelihoodType logl_proposal(proposal.log_likelihood(),
												logl.get_tiebreaker());

	// Do the proposal for the tiebreaker
	log_H += logl_proposal.perturb(rng);

	if(log_H > 0.)
		log_H = 0.;

	// Make a LikelihoodType for the proposal
	if(rng.rand() <= exp(log_H) && level.get_log_likelihood() < logl_proposal)
	{
		particle = proposal;
		logl = logl_proposal;
		level.increment_accepts(1);
	}
	level.increment_tries(1);

	// Count visits and exceeds
	if(level_assignments[which] != (_levels.size()-1))
	{
		level.increment_visits(1);
		if(_levels[level_assignments[which+1]].get_log_likelihood() < 
						log_likelihoods[which])
			level.increment_exceeds(1);
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
						+ static_cast<int>(pow(10., 2.*rng.rand()));

	// If the proposal was to not move, go +- one level
	if(proposal == static_cast<int>(level_assignments[which]))
		proposal = ((rng.rand() < 0.5)?(proposal-1):(proposal+1));

	// Wrap into allowed range
	proposal = DNest4::mod(proposal, static_cast<int>(_levels.size()));

	// Acceptance probability
	double log_A = -_levels[proposal].get_log_X()
					+ _levels[level_assignments[which]].get_log_X();

	// Pushing up part
	log_A += log_push(thread, proposal) - log_push(thread, level_assignments[which]);

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
		level_assignments[which] = static_cast<int>(proposal);
	}
}

template<class ModelType>
void Sampler<ModelType>::run()
{
	do_mcmc();
}

template<class ModelType>
double Sampler<ModelType>::log_push(unsigned int thread,
										unsigned int which_level) const
{
	// Reference to this thread's copy of levels
	const std::vector<Level>& _levels = copies_of_levels[thread];

	assert(which_level < _levels.size());
	if(_levels.size() == options.max_num_levels)
		return 0.;

	int i = which_level - (static_cast<int>(_levels.size()) - 1);
	return static_cast<double>(i)/options.lambda;
}

} // namespace DNest4

