#ifndef DNest4_Jeffreys
#define DNest4_Jeffreys

#include "ContinuousDistribution.h"
#include "../RNG.h"

namespace DNest4
{

/*
* Jeffreys distributions (reciprocal)
*/
class Jeffreys:public ContinuousDistribution
{
    private:
        // Support is defined between lower and upper
        double lower, upper;
        double d;

    public:
        Jeffreys(double lower=1.0, double upper=100.0);

        double cdf(double x) const;
        double cdf_inverse(double x) const;
        double log_pdf(double x) const;
};


class ModifiedJeffreys:public ContinuousDistribution
{
    private:
        // Support is defined between 0 and upper
        // pdf is uniform for x<<knee and reciprocal for x>>knee
        double knee, upper;
        double d;

    public:
        ModifiedJeffreys(double knee=1.0, double upper=100.0);

        double cdf(double x) const;
        double cdf_inverse(double x) const;
        double log_pdf(double x) const;
};


} // namespace DNest4

#endif

