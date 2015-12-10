#ifndef DNest4_Sampler
#define DNest4_Sampler

#include <vector>
#include <list>
#include <thread>
#include "LikelihoodType.h"
#include "Options.h"
#include "Level.h"
#include "RNG.h"

namespace DNest4
{

template<class ModelType>
class Sampler
{
	private:
		// Number of threads and compression
		unsigned int num_threads;
		double compression;

		// Options (most useful comment ever)
		Options options;

		// Particles
		std::vector<ModelType> particles;

		// Levels
		std::list<LikelihoodType> levels;

		// Random number generators
		std::vector<RNG> rngs;

	public:
		// Constructor: Pass in Options object
		Sampler(unsigned int num_threads,
						double compression, const Options& options);

		// Set rng seeds, then draw all particles from the prior
		void initialise(unsigned int first_seed);
};

} // namespace DNest4

#include "SamplerImpl.h"
#endif

