#ifndef DNest4_StraightLine
#define DNest4_StraightLine

#include "DNest4/code/DNest4.h"
#include <valarray>
#include <ostream>

class StraightLine
{
	private:
		// The slope and intercept
		double m, b;

		// Noise sd
		double sigma;

		// Model prediction
		std::valarray<double> mu;

		// Compute the model line given the current values of m and b
		void calculate_mu();

	public:
		// Constructor
		StraightLine();

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

