#ifndef DNest4_Uniform
#define DNest4_Uniform

#include <iostream>
#include "ContinuousDistribution.h"
#include "../RNG.h"

namespace DNest4
{

/*
* Uniform distribution
*/
class Uniform:public ContinuousDistribution
{
    private:
        double lower, upper;

    public:
        Uniform(double lower=0.0, double upper=1.0);

        double cdf(double x) const;
        double cdf_inverse(double x) const;
        double log_pdf(double x) const;
        // ostream representation of Uniform class
        virtual std::ostream& print(std::ostream& out) const override
        {
            out << "Uniform(" << lower << "; " << upper << ")";
            return out;
        }
};



} // namespace DNest4

#endif

