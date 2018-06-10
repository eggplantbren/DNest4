#ifndef DNest4_LogUniform
#define DNest4_LogUniform

#include "ContinuousDistribution.h"
#include "../RNG.h"

namespace DNest4
{

/*
* LogUniform distributions (reciprocal)
*/
class LogUniform:public ContinuousDistribution
{
    private:
        // Support is defined between lower and upper
        double lower, upper;
        double d;

    public:
        LogUniform(double _lower=1.0, double _upper=100.0);

        double cdf(double x) const;
        double cdf_inverse(double x) const;
        double log_pdf(double x) const;
};


class ModifiedLogUniform:public ContinuousDistribution
{
    private:
        // Support is defined between 0 and upper
        // pdf is uniform for x<<knee and reciprocal for x>>knee
        double knee, upper;
        double d;

    public:
        ModifiedLogUniform(double _knee=1.0, double _upper=100.0);

        double cdf(double x) const;
        double cdf_inverse(double x) const;
        double log_pdf(double x) const;
};


} // namespace DNest4

#endif

