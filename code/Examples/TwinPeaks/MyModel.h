#ifndef DNest4_Template_MyModel
#define DNest4_Template_MyModel

#include <ostream>
#include <vector>
#include "DNest4/code/DNest4.h"
#include "TwinPeaksModel.h"

enum class Mode { first, second, sum };

class MyModel
{
    private:

        static Mode mode;
        static std::vector<double> fs, gs;

        TwinPeaksModel x;

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

        // Set the mode
        static void set_mode(const Mode& new_mode);

        // Load data from first.txt and second.txt
        static void load_data();
};

#endif

