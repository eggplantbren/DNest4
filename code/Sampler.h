#ifndef DNest4_Sampler
#define DNest4_Sampler

#include <vector>
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

		// Particles, tiebreaker values, and level assignments
		std::vector<ModelType> particles;
		std::vector<double> log_likelihoods;
		std::vector<double> tiebreakers;
		std::vector<unsigned int> level_assignments; // j in the paper

		// Levels
		std::vector<Level> levels;
		std::vector< std::vector<Level> > copies_of_levels;

		// Random number generators
		std::vector<RNG> rngs;

		// Log likelihood values accumulated (to create a new level)
		std::vector<LikelihoodType> log_likelihood_keep;

		// Number of saved particles
		unsigned int saves;

		/* Private methods */
		// Do an MCMC step of particle 'which' on thread 'thread'
		void update_particle(unsigned int thread, unsigned int which);

		// Do an MCMC step of the level assignment of particle 'which' on thread
		// 'thread'
		void update_level_assignment(unsigned int thread, unsigned int which);

		// Do MCMC for a while on thread 'thread'
		void do_mcmc_thread(unsigned int thread);

		// Do MCMC for a while on multiple threads
		// then come together and do book-keeping
		void do_mcmc();

		// Weighting function
		double log_push(unsigned int thread, unsigned int which_level) const;

	public:
		// Constructor: Pass in Options object
		Sampler(unsigned int num_threads,
						double compression, const Options& options);

		// Set rng seeds, then draw all particles from the prior
		void initialise(unsigned int first_seed);

		// Launch everything
		void run();
};

} // namespace DNest4

#include "SamplerImpl.h"
#endif

