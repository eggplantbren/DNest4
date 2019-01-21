#ifndef DNest4_Template_MyModel
#define DNest4_Template_MyModel

#include "DNest4/code/DNest4.h"
#include <ostream>
#include <Rcpp.h>
#include <RInside.h>

class MyModel
{
    private:

        // Embedded R instance
        static RInside R;

        // Parameter vector with Uniform(0, 1) priors
        std::vector<double> params;

    public:
        // Constructor only gives size of params
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

