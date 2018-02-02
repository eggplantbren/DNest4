#ifndef DNest4_Laplace
#define DNest4_Laplace

#include "ContinuousDistribution.h"
#include "../RNG.h"
#include <limits>

namespace DNest4
{

/*
* Laplace distributions
*/
class Laplace:public ContinuousDistribution
{
    private:
        // Location and scale parameter
        double center, width;

    public:
        Laplace(double center=0.0, double width=1.0);
        // setter
        void setpars(double center, double width);

        double cdf(double x) const;
        double cdf_inverse(double x) const;
        double log_pdf(double x) const;

        // Sign function
        static int sign(double x);
};

} // namespace DNest4

#endif

