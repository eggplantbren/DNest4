#ifndef DNest4_Options
#define DNest4_Options

#include <string>
#include <ostream>
#include <istream>

namespace DNest4
{

/*
* An object of this class represents a set of options for the sampler,
* typically read in from an OPTIONS file.
*/
class Options
{
	// All Sampler specialisations can access Options members
	template<class ModelType>
	friend class Sampler;

	private:
		// Numerical options
		unsigned int num_particles;
		unsigned int new_level_interval;
		unsigned int save_interval;
		unsigned int thread_steps;
		unsigned int max_num_levels;
		double lambda, beta;
		unsigned int max_num_saves;

		// Filenames
		std::string sample_file;
		std::string sample_info_file;
		std::string levels_file;

	public:
		Options() {};
		Options(unsigned int num_particles,
			unsigned int new_level_interval,
			unsigned int save_interval,
			unsigned int thread_steps,
			unsigned int max_num_levels,
			double lambda,
			double beta,
			unsigned int max_num_saves);

		Options(const char* filename);
		void load(const char* filename);

		void print(std::ostream& out) const;
		void read(std::istream& in);
};

} // namespace DNest4

// Operator << which just calls print
std::ostream& operator << (std::ostream& out, const DNest4::Options& o);
std::istream& operator >> (std::istream& in, DNest4::Options& o);

#endif

