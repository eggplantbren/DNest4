#include <cstdlib>
#include <iostream>
#include "CommandLineOptions.h"
#include "RNG.h"
#include "Sampler.h"
#include "Options.h"

namespace DNest4
{

template<class ModelType>
Sampler<ModelType> setup(int argc, char** argv)
{
	CommandLineOptions options(argc, argv);
	return setup<ModelType>(options);
}

template<class ModelType>
Sampler<ModelType> setup(const CommandLineOptions& options)
{
	// Load sampler options from file
	Options sampler_options(options.get_options_file().c_str(),
									options.get_use_gzip());

	// Create sampler
	Sampler<ModelType> sampler(options.get_num_threads(),
								options.get_compression_double(), sampler_options);

	// Seed RNGs
	sampler.initialise(options.get_seed_uint());

	return sampler;
}

template<class ModelType>
void start(int argc, char** argv)
{
	CommandLineOptions options(argc, argv);
	Sampler<ModelType> sampler = setup<ModelType>(options);
	sampler.run();
}

} // namespace DNest4

