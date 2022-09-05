#ifndef DNest4_CommandLineOptions
#define DNest4_CommandLineOptions

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
		std::string options_file;
		std::string seed;
		std::string data_file;
		std::string compression;
		int num_threads;
        std::string config_file;
        bool adaptive;        

	public:
		CommandLineOptions(int argc, char** argv);

		const std::string& get_options_file() const
		{ return options_file; }

		const std::string& get_seed() const
		{ return seed; }

		const std::string& get_data_file() const
		{ return data_file; }

		const std::string& get_compression() const
		{ return compression; }

		int get_num_threads() const
		{ return num_threads; }

                const std::string& get_config_file() const
                { return config_file; }

        bool get_adaptive() const
        { return adaptive; }

		// Convert seed string to an unsigned integer and return it
		unsigned int get_seed_uint() const;

		// Convert compression to a double and return it
		double get_compression_double() const;

		// Print help message
		void print_help() const;

};

} // namespace DNest4

#endif

