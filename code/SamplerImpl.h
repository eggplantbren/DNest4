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
,particles(options.num_particles)
,levels(1, LikelihoodType())
,rngs(num_threads)
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
	for(ModelType& m: particles)
		m.from_prior(rngs[0]);
	std::cout<<"done."<<std::endl<<std::endl;
}

} // namespace DNest4

