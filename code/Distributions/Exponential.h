#ifndef DNest4_Exponential
#define DNest4_Exponential

#include "ContinuousDistribution.h"
#include "../RNG.h"
#include <limits>

namespace DNest4
{

/*
* Exponential distribution
*/
class Exponential:public ContinuousDistribution
{
    private:
        double scale; // scale parameter

    public:
        Exponential(double scale=1.0);
        void setpars(double scale); // setter

        double cdf(double x) const;
        double cdf_inverse(double p) const;
        double log_pdf(double x) const;
};

/*
* truncated Exponential distribution
*/
class TruncatedExponential:public ContinuousDistribution
{
    private:
        double scale; // scale parameter
        double lower, upper; // truncation bounds
        Exponential unE; // the original, untruncated, Exponential distribution
        double c;

    public:
        TruncatedExponential(double scale=1.0, double lower=0., double upper=1./0.);
        void setpars(double scale); // setter

        double cdf(double x) const;
        double cdf_inverse(double p) const;
        double log_pdf(double x) const;
};


} // namespace DNest4

#endif

