#ifndef DNest4_Cauchy
#define DNest4_Cauchy

#include "ContinuousDistribution.h"
#include "../RNG.h"
#include <limits>

namespace DNest4
{

/*
* Cauchy distributions
*/
class Cauchy:public ContinuousDistribution
{
    private:
        // Location and scale parameter
        double center, width;

    public:
        Cauchy(double center=0.0, double width=1.0);

        double cdf(double x) const;
        double cdf_inverse(double x) const;
        double log_pdf(double x) const;
};

} // namespace DNest4

#endif

