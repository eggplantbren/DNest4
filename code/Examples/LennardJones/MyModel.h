#ifndef DNest4_Template_MyModel
#define DNest4_Template_MyModel

#include "DNest4/code/DNest4.h"
#include <ostream>

class MyModel
{
    private:
        // Length of the box
        static constexpr double L = 50.0;

        // The number of atoms
        static constexpr size_t num_atoms = 38;

        // Positions of the atoms
        std::vector<double> x;
        std::vector<double> y;
        std::vector<double> z;

        // Potential energy of pairs
        std::vector<std::vector<double>> PE;

        // Compute all PEs
        void compute_PE();

        // Compute a single PE
        void compute_PE(size_t i, size_t j);

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

