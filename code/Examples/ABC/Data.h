#ifndef DNest4_ABC_Data
#define DNest4_ABC_Data

#include <vector>

class Data
{
	private:
		std::vector<double> x;

		// Summaries
		double x_min, x_max;

	public:
		Data();
		void load(const char* filename);
		const std::vector<double>& get_x() const { return x; }
		double get_x_min() const { return x_min; }
		double get_x_max() const { return x_max; }

	// Singleton
	private:
		static Data instance;
	public:
		static Data& get_instance() { return instance; }
};

#endif

