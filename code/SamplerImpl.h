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
,tiebreakers(options.num_particles*num_threads)
,level_assignments(options.num_particles*num_threads, 0)
,levels(1, LikelihoodType())
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
		particles[i].from_prior(rngs[0]);
		log_likelihoods[i] = particles[i].log_likelihood();
		tiebreakers[i] = rng.rand();
	}
	std::cout<<"done."<<std::endl<<std::endl;
}

template<class ModelType>
void Sampler<ModelType>::do_mcmc_thread(unsigned int thread,
										std::vector<Level>& levels_copy)
{
	// Copy the levels, accumulate results into the copy
	levels_copy = levels;

	// Reference to the RNG for this thread
	RNG& rng = rngs[thread];

	// Store log likelihood values observed
	std::vector<LikelihoodType> keep(options.thread_steps);

	// First particle belonging to this thread
	const int start_index = thread*options.num_particles;

	// Do some MCMC
	int which;
	for(unsigned int i=0; i<options.thread_steps; ++i)
	{
		which = start_index + rng.rand_int(options.num_particles);
		if(rng.rand() <= 0.5)
		{
			update(which, thread, levels_copy);
			update_level_assignment(which, thread);
		}
		else
		{
			update_level_assignment(which, thread);
			update(which, thread, levels_copy);
		}
	}
}

template<class ModelType>
void Sampler<ModelType>::do_mcmc()
{
	std::vector<std::thread> threads;
	std::vector< std::vector<Level> > levels_copies(num_threads, levels);

	for(unsigned int i=0; i<num_threads; ++i)
		threads.push_back(std::thread(std::bind(&Sampler<ModelType>::do_mcmc_thread, this, i, std::ref(levels_copies[i]))));
	for(std::thread& t: threads)
		t.join();

	// Go through level copies
	std::vector<Level> levels_orig = levels;
	for(const auto& lcps: levels_copies)
	{
		for(size_t i=0; i<levels.size(); ++i)
		{
			levels[i].increment_accepts(lcps[i].get_accepts()
												- levels_orig[i].get_accepts());
			levels[i].increment_tries(lcps[i].get_tries()
												- levels_orig[i].get_tries());
			levels[i].increment_visits(lcps[i].get_visits()
												- levels_orig[i].get_visits());
			levels[i].increment_exceeds(lcps[i].get_exceeds()
												- levels_orig[i].get_exceeds());
		}
	}

	
}

template<class ModelType>
void Sampler<ModelType>::update(unsigned int which, unsigned int thread,
							std::vector<Level>& levels_copy)
{
	// Reference to the RNG for this thread
	RNG& rng = rngs[thread];

	// Reference to the level we're in
	Level& level = levels_copy[level_assignments[which]];

	// Reference to the particle being moved
	ModelType& p = particles[which];
	double& logl = log_likelihoods[which];
	double& tb = tiebreakers[which];

	// Do the proposal for the particle
	ModelType proposal = p;
	double log_H = proposal.perturb(rng);
	if(log_H > 0.)
		log_H = 0.;
	double logl_proposal = proposal.log_likelihood();

	// Do the proposal for the tiebreaker
	double tiebreaker_proposal = tb;
	tiebreaker_proposal += rng.randh();
	wrap(tiebreaker_proposal, 0., 1.);

	// Make a LikelihoodType for the proposal
	LikelihoodType prop(logl_proposal, tiebreaker_proposal);
	if(rng.rand() <= exp(log_H) && level.get_log_likelihood() < prop)
	{
		p = proposal;
		logl = logl_proposal;
		tb = tiebreaker_proposal;
		level.increment_accepts(1);
	}
	level.increment_tries(1);

	// Count visits and exceeds
	if(which != (levels.size()-1))
	{
		level.increment_visits(1);
		LikelihoodType temp(logl, tb);
		if(levels[level_assignments[which+1]].get_log_likelihood() < temp)
			level.increment_exceeds(1);
	}
}

template<class ModelType>
void Sampler<ModelType>::update_level_assignment(unsigned int which,
													unsigned int thread)
{
	// Reference to the RNG for this thread
	RNG& rng = rngs[thread];

	// Generate proposal
	int proposal = static_cast<int>(level_assignments[which])
						+ static_cast<int>(pow(10., 2.*rng.rand()));

	// If the proposal was to not move, go +- one level
	if(proposal == static_cast<int>(level_assignments[which]))
		proposal = ((rng.rand() < 0.5)?(proposal-1):(proposal+1));

	// Wrap into allowed range
	proposal = DNest4::mod(proposal, static_cast<int>(levels.size()));

	// Acceptance probability
	double log_A = -levels[proposal].get_log_X()
					+ levels[level_assignments[which]].get_log_X();

	// Pushing up part
	log_A += log_push(proposal) - log_push(level_assignments[which]);

	// Enforce uniform exploration part (if all levels exist)
	if(levels.size() == options.max_num_levels)
		log_A += options.beta*log((double)(levels[level_assignments[which]].get_tries() + 1)/(double)(levels[proposal].get_tries() + 1));

	// Prevent exponentiation of huge numbers
	if(log_A > 0.)
		log_A = 0.;

	// Make a LikelihoodType for the proposal
	LikelihoodType prop(log_likelihoods[which], tiebreakers[which]);
	if(rng.rand() <= exp(log_A) && levels[proposal].get_log_likelihood() < prop)
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
double Sampler<ModelType>::log_push(unsigned int which_level) const
{
	assert(which_level < levels.size());
	if(levels.size() == options.max_num_levels)
		return 0.;

	int i = which_level - (static_cast<int>(levels.size()) - 1);
	return static_cast<double>(i)/options.lambda;
}

} // namespace DNest4

