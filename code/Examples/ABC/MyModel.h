#ifndef DNest4_ABC_MyModel
#define DNest4_ABC_MyModel

#include <vector>
#include <ostream>
#include "DNest4/code/DNest4.h"

class MyModel
{
	private:
		double mu, log_sigma;
		std::vector<double> n;


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

