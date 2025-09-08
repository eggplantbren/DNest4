#ifndef DNest4_Template_MyModel
#define DNest4_Template_MyModel

#include "DNest4/code/DNest4.h"
#include <ostream>

class MyModel:public DNest4::Hypercube<MyModel>
{
    private:
        double x, y;

    public:

        MyModel();

        // Transform us to xs
        void prior_transform();

        // Likelihood function
        double log_likelihood() const;

        // Print function
        void print(std::ostream& out) const;
};

#endif

