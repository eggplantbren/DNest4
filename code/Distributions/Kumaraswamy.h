#ifndef DNest4_Kumaraswamy
#define DNest4_Kumaraswamy

#include "ContinuousDistribution.h"
#include "../RNG.h"

namespace DNest4
{

/*
* Kumaraswamy distribution, a Beta-like distribution
* https://en.wikipedia.org/wiki/Kumaraswamy_distribution
*/
class Kumaraswamy:public ContinuousDistribution
{
    private:
        double a, b;

    public:
        Kumaraswamy(double a=1.0, double b=1.0);

        double cdf(double x) const;
        double cdf_inverse(double p) const;
        double log_pdf(double x) const;
};

} // namespace DNest4

#endif

