#ifndef DNest4_Triangular
#define DNest4_Triangular

#include "ContinuousDistribution.h"
#include "../RNG.h"

namespace DNest4
{

/*
* Triangular distribution
*/
class Triangular:public ContinuousDistribution
{
    private:
        double lower, centre, upper;

    public:
        Triangular(double lower=0.0, double centre=0.0, double upper=1.0);

        double cdf(double x) const;
        double cdf_inverse(double x) const;
        double log_pdf(double x) const;
};



} // namespace DNest4

#endif
