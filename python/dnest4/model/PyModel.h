#ifndef DNest4_PyModel
#define DNest4_PyModel

#include <ostream>
#include <valarray>
#include "DNest4.h"

class PyModel
{
    private:
        // The mean
        double m;

        // Noise sd
        double sigma;

    public:
        // Constructor
        PyModel();

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

