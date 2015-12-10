#include <cassert>
#include <iostream>

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
	// Seed the RNGs, incrementing the seed each time
	for(RNG& rng: rngs)
		rng.set_seed(first_seed++);

	std::cout<<"# Generating "<<options.num_particles;
	std::cout<<" from the prior..."<<std::flush;
	for(size_t i=0; i<particles.size(); ++i)
	{
		particles[i].from_prior(rngs[0]);
		log_likelihoods[i] = LikelihoodType(particles[i].get_log_likelihood(),
												rngs[0].rand());
	}
	std::cout<<"done."<<std::endl<<std::endl;
}

template<class ModelType>
std::vector<LikelihoodType> Sampler<ModelType>::do_mcmc(unsigned int thread)
{
	// Store log likelihood values observed
	std::vector<LikelihoodType> keep(options.thread_steps);

	// First particle belonging to this thread
	int start_index = thread*options.num_particles;

	// Do this many moves
	for(unsigned int i=0; i<options.thread_steps; ++i)
	{
		which = start_index + rngs[thread].rand_int(options.num_particles);
		update(particles[which], thread);
		keep[i] = particles[which].get_log_likelihood();
	}
}

} // namespace DNest4

