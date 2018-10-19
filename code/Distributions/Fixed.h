#ifndef DNest4_Fixed
#define DNest4_Fixed

#include "ContinuousDistribution.h"
#include "../RNG.h"

namespace DNest4
{

/*
* Not a real distribution, just a fixed parameter
*/
class Fixed:public ContinuousDistribution
{
    private:
        double val;

    public:
        Fixed(double val=0.0);

        double cdf(double x) const;
        double cdf_inverse(double p) const;
        double log_pdf(double x) const;
        // this special class reimplements perturb to save some work
        double perturb(double& x, RNG& rng) const;
};

} // namespace DNest4

#endif

