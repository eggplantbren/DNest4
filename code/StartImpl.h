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
	std::cout<<"# Using "<<options.get_num_threads()<<" thread"<<
		((options.get_num_threads() == 1)?("."):("s."))<<std::endl;

	std::cout<<"# Target compression factor between levels = ";
	std::cout<<options.get_compression()<<std::endl;

	// Load sampler options from file
	Options sampler_options(options.get_options_file().c_str());

	// Create sampler
	Sampler<ModelType> sampler(options.get_num_threads(),
								options.get_compression_double(),
								sampler_options,
								true, options.get_adaptive());

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

template<class ModelType>
void start(const CommandLineOptions& options)
{
	Sampler<ModelType> sampler = setup<ModelType>(options);
	sampler.run();
}

} // namespace DNest4

