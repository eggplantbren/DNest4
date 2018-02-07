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

class TruncatedCauchy:public ContinuousDistribution
{
    private:
        double center, width; // Location and scale parameter
        double lower, upper; // truncation bounds
        Cauchy unC; // the original, untruncated, Cauchy distribution
        double c;

    public:
        TruncatedCauchy(double center=0.0, double width=1.0,
                        double lower=-std::numeric_limits<double>::infinity(),
                        double upper=std::numeric_limits<double>::infinity());

        double cdf(double x) const;
        double cdf_inverse(double p) const;
        double pdf(double x) const;
        double log_pdf(double x) const;
        double rvs(RNG& rng) const;
};


} // namespace DNest4

#endif

