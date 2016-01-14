#ifndef DNest4_GalaxyField_MyModel
#define DNest4_GalaxyField_MyModel

#include <vector>
#include "RJObject/RJObject.h"
#include "MyConditionalPrior.h"

class MyModel
{
	private:
		DNest4::RJObject<MyConditionalPrior> objects;

		// The model image
		std::vector< std::vector<long double> > image;
		void calculate_image();

		// How many steps since image was computed from scratch
		int staleness;

		// Noise standard deviation
		double sigma;

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

