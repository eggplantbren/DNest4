#include <cassert>
#include <iostream>
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
,levels(1, LikelihoodType())
,rngs(num_threads)
,log_likelihood_keep()
{
	assert(num_threads >= 1);
	assert(compression > 1.);
}

template<class ModelType>
void Sampler<ModelType>::initialise(unsigned int first_seed)
{
	// Reference to an RNG to use
	RNG& rng = rngs[0];

	// Seed the RNGs, incrementing the seed each time
	for(RNG& rng: rngs)
		rng.set_seed(first_seed++);

	std::cout<<"# Generating "<<options.num_particles;
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
std::vector<LikelihoodType> Sampler<ModelType>::do_mcmc(unsigned int thread)
{
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
		update(which, thread);
	}

	return keep;
}

template<class ModelType>
void Sampler<ModelType>::update(unsigned int which, unsigned int thread)
{
	// Reference to the RNG for this thread
	RNG& rng = rngs[thread];

	// Reference to the particle being moved
	ModelType& p = particles[which];
	double& tb = tiebreakers[which];

	// Do the proposal for the particle
	ModelType proposal = p;
	double log_H = p.perturb();
	if(log_H > 0.)
		log_H = 0.;

	// Do the proposal for the tiebreaker
	double tiebreaker_proposal = tb;
	tiebreaker_proposal += rng.randh();
	wrap(tiebreaker_proposal, 0., 1.);

	// Make a LikelihoodType for the proposal
	LikelihoodType prop(p.log_likelihood(), tiebreaker_proposal);

	if(rng.rand() <= exp(log_H) &&
			levels[level_assignments[which]] < prop)
	{
		p = proposal;
		tb = tiebreaker_proposal;
	}
}

} // namespace DNest4

