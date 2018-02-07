#ifndef DNest4_Distribution
#define DNest4_Distribution

#include "../RNG.h"

namespace DNest4
{

/*
* An abstract base class for continuous probability distributions.
*/
class ContinuousDistribution
{
    private:

    public:
        double generate(RNG& rng) const;
        double perturb(double& x, RNG& rng) const;

        virtual void setpars(double) {};
        virtual void setpars(double, double) {};

        virtual double cdf(double x) const = 0;
        virtual double cdf_inverse(double x) const = 0;
        virtual double log_pdf(double x) const = 0;
};

} // namespace DNest4

#endif

