#ifndef DNest4_JuliaModel_MyModel
#define DNest4_JuliaModel_MyModel

#include "DNest4/code/DNest4.h"
#include <vector>
#include <ostream>

class MyModel
{
	private:
		std::vector<double> x;

	public:
		MyModel();

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

