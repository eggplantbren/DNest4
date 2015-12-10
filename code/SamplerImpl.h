#include <cassert>

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


} // namespace DNest4

