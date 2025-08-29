#ifndef DNest4_Template_MyModel
#define DNest4_Template_MyModel

#include "DNest4/code/DNest4.h"
#include <ostream>

class MyModel:public DNest4::Hypercube<5, MyModel>
{
    private:


    public:
        // Constructor only gives size of params
        MyModel();

        // Transform us to xs
        void prior_transform();

        // Likelihood function
        double log_likelihood() const;

        // Print to stream
        void print(std::ostream& out) const;

        // Return string with column information
        std::string description() const;
};

#endif

