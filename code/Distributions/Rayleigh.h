#ifndef DNest4_Rayleigh
#define DNest4_Rayleigh

#include "ContinuousDistribution.h"
#include "../RNG.h"

namespace DNest4
{

/*
* Rayleigh distributions
*/
class Rayleigh:public ContinuousDistribution
{
    private:
        double scale;

    public:
        Rayleigh(double scale=1.0);

        double cdf(double x) const;
        double cdf_inverse(double x) const;
        double log_pdf(double x) const;
        // ostream representation of Rayleigh class
        virtual std::ostream& print(std::ostream& out) const override
        {
            out << "Rayleigh(" << scale << ")";
            return out;
        }

};

class TruncatedRayleigh:public ContinuousDistribution
{
    private:
        double scale;  // scale parameter
        double lower, upper;  // lower and upper truncation boundaries
        double lcdf, ucdf;
        double tp, logtp;

    public:
        TruncatedRayleigh(double scale=1.0, double lower=0.0, double upper=1.0);

        double cdf(double x) const;
        double cdf_inverse(double x) const;
        double log_pdf(double x) const;
        // ostream representation of TruncatedRayleigh class
        virtual std::ostream& print(std::ostream& out) const override
        {
            out << "TruncatedRayleigh(" << scale << "; [" << lower << " , " << upper << "])";
            return out;
        }
};


} // namespace DNest4

#endif

