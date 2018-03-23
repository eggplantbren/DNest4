#ifndef DNest4_Gaussian
#define DNest4_Gaussian

#include "ContinuousDistribution.h"
#include "../RNG.h"
#include "../Utils.h"

namespace DNest4
{

class Gaussian:public ContinuousDistribution
{
    private:
        // Location and scale parameter
        double center, width;
        double _norm_pdf_logC = log(sqrt(2*M_PI));

    public:
        Gaussian(double center=0.0, double width=1.0);

        double cdf(double x) const;
        double cdf_inverse(double x) const;
        double log_pdf(double x) const;

};

} // namespace DNest4

#endif

