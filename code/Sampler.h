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
		std::vector<LikelihoodType> log_likelihoods;
		std::vector<unsigned int> level_assignments; // j in the paper

		// Levels
		std::vector<Level> levels;
		std::vector< std::vector<Level> > copies_of_levels;

		// Random number generators
		std::vector<RNG> rngs;

		// Number of saved particles
		unsigned int saves;

		/* Private methods */
		// Do an MCMC step of particle 'which' on thread 'thread'
		void update_particle(unsigned int thread, unsigned int which);

		// Do an MCMC step of the level assignment of particle 'which' on thread
		// 'thread'
		void update_level_assignment(unsigned int thread, unsigned int which);

		// Do MCMC for a while on thread 'thread'
		void mcmc_thread(unsigned int thread, std::vector<LikelihoodType>& keep);

		// Do MCMC for a while on multiple threads
		// then come together and do book-keeping
		std::vector<LikelihoodType> do_some_mcmc();

		// Add new levels, save output files, etc
		void do_bookkeeping(std::vector<LikelihoodType>& keep);

		// Create one level
		void create_level(std::vector<LikelihoodType>& keep);

		// Weighting function
		double log_push(unsigned int thread, unsigned int which_level) const;

		// Functions to do with the output files
		void initialise_output_files() const;
		void save_levels() const;
		void save_particle() const;

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

