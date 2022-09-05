#ifndef DNest4_Rosenbrock
#define DNest4_Rosenbrock

#include "DNest4/code/DNest4.h"
#include <ostream>
#include <vector>


class Rosenbrock
{
    private:

        // Constants
        static constexpr int n1 = 3;
        static constexpr int n2 = 2;
        static constexpr double a = 1.0/20.0;
        static constexpr double b = 100.0/20.0;
        static constexpr double mu = 1.0;

	private:
		std::vector<std::vector<double>> xs;

	public:
		// Constructor only gives size of params
		Rosenbrock();

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

