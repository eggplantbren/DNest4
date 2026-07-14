#ifndef DNest4_GaussianExample
#define DNest4_GaussianExample

#include "DNest4/code/DNest4.h"
#include <valarray>
#include <ostream>

class GaussianExample
{
	private:
		std::valarray<double> params;
		double width = 10;
		int coordinate = 0;

	public:
		// Constructor only gives size of params
		GaussianExample();

		// return analytic solution to log_Z
		double analytic_log_Z();

		// Generate the point from the prior
		void from_prior(DNest4::RNG& rng);

		// Metropolis-Hastings proposals
		double perturb(DNest4::RNG& rng);

		// Likelihood function
		double log_likelihood() const;

		// Print to stream
		void print(std::ostream& out) const;

		// Return string with column information
		std::string description() const;
};

#endif

