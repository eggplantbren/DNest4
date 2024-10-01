#include "CommandLineOptions.h"

#include <ctime>
#include <sstream>
#include <iostream>
#include <unistd.h>	// for getopt
#include <cstdlib>	// for exit

namespace DNest4
{
  
CommandLineOptions::CommandLineOptions(int argc, char** argv)
:options_file("OPTIONS")
,seed("time")
,data_file("")
,compression("2.7182818284590451")
,num_threads(1)
,config_file("")
{
	// The following code is based on the example given at
	// http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt

	int c;
	std::stringstream s;

	opterr = 0;
	while((c = getopt(argc, argv, "ho:s:d:c:t:f:")) != -1)
	switch(c)
	{
		case 'h':
			print_help();
			break;
		case 'o':
			options_file = std::string(optarg);
			break;
		case 's':
			seed = std::string(optarg);
			break;
		case 'd':
			data_file = std::string(optarg);
			break;
		case 'c':
			compression = std::string(optarg);
			break;
		case 't':
			s<<optarg;
			s>>num_threads;
			break;
		case 'f':
			config_file = std::string(optarg);
			break;
		case '?':
			std::cerr<<"# Option "<<optopt<<" requires an argument."<<std::endl;
			if(isprint(optopt))
				std::cerr<<"# Unknown option "<<optopt<<"."<<std::endl;
			else
				std::cerr<<"# Unknown option character "<<optopt<<"."<<std::endl;
			exit(0);
		default:
			abort();
           }

	for(int index = optind; index < argc; index++)
		std::cout<<"# Non-option argument "<<argv[index]<<std::endl;

	if(num_threads <= 0)
	{
		std::cerr<<"# Invalid number of threads: "<<num_threads<<"."<<std::endl;
		num_threads = 1;
	}
}

double CommandLineOptions::get_compression_double() const
{
	double val;
	std::stringstream s(compression);
	s>>val;
	return val;
}

unsigned int CommandLineOptions::get_seed_uint() const
{
	if(seed.compare("time") == 0)
		return static_cast<unsigned int>(time(0));

	unsigned int i;
	std::stringstream s(seed);
	s>>i;
	return i;
}

void CommandLineOptions::print_help() const
{
	std::cout<<"DNest4 Command Line Options: "<<std::endl;
	std::cout<<"-h: display this message"<<std::endl;
	std::cout<<"-o <filename>: load DNest4 options from the specified file. Default=OPTIONS"<<std::endl;
	std::cout<<"-s <seed>: seed the random number generator with the specified value. If unspecified, the system time is used."<<std::endl;
	std::cout<<"-d <filename>: Load data from the specified file, if required."<<std::endl;
	std::cout<<"-c <value>: Specify a compression value (between levels) other than e."<<std::endl;
	std::cout<<"-t <num_threads>: run on the specified number of threads. Default=1."<<std::endl;
	std::cout<<"-f <filename>: a custom configuration file for adding problem specific options if required."<<std::endl;
	exit(0);
}

} // namespace DNest4

