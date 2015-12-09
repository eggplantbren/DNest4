#ifndef _CommandLineOptions_
#define _CommandLineOptions_

#include <string>

namespace DNest4
{

/*
* A little class containing all of the command line options.
* Uses GNU GetOpt to parse the input.
*/

class CommandLineOptions
{
	private:
		std::string levels_file;
		std::string options_file;
		std::string seed;
		std::string data_file;
		std::string compression;
		int numThreads;
		std::string config_file;
		bool use_gzip;
                
	public:
		CommandLineOptions(int argc, char** argv);

		// Getters
		const std::string& get_levels_file() const
		{ return levels_file; }

		const std::string& get_options_file() const
		{ return options_file; }

		const std::string& get_seed() const
		{ return seed; }

		const std::string& get_data_file() const
		{ return data_file; }

		const std::string& get_compression() const
		{ return compression; }

		int get_numThreads() const
		{ return numThreads; }

		const std::string& get_config_file() const
		{ return config_file; }

		bool get_use_gzip() const
		{ return use_gzip; }

		// Convert seed string to an unsigned integer and return it
		unsigned int get_seed_uint() const;

		// Convert compression to a double and return it
		double get_compression_double() const;

		// Print help message
		void printHelp() const;

};

} // namespace DNest4
#include "CommandLineOptionsImpl.h"

#endif

