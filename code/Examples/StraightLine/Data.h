#ifndef DNest4_Data
#define DNest4_Data

#include <valarray>

/*
* An object of this class is a dataset
*/
class Data
{
	private:
		// The data points
		std::valarray<double> x;
		std::valarray<double> y;

	public:
		// Constructor
		Data();

		// Load data from a file
		void load(const char* filename);

		// Access to the data points
		const std::valarray<double>& get_x() const
		{ return x; }
		const std::valarray<double>& get_y() const
		{ return y; }

	private:
		// Static "global" instance
		static Data instance;

	public:
		// Getter for the global instance
		static Data& get_instance()
		{ return instance; }
};

#endif

