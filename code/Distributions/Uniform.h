#ifndef DNest4_Uniform
#define DNest4_Uniform

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
};



} // namespace DNest4

#endif

