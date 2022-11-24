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
        // ostream representation of Gaussian class
        virtual std::ostream& print(std::ostream& out) const override
        {
            out << "Gaussian(" << center << "; " << width << ")";
            return out;
        }

};

/*
* truncated Gaussian distribution
*/
class TruncatedGaussian:public ContinuousDistribution
{
    private:
        // Location and scale parameter
        double center, width;
        double lower, upper; // truncation bounds
        Gaussian unG; // the original, untruncated, Gaussian distribution
        double _norm_pdf_logC = log(sqrt(2*M_PI));
        double c;

    public:
        TruncatedGaussian(double center=0.0, double width=1.0, double lower=-1./0., double upper=1./0.);

        double cdf(double x) const;
        double cdf_inverse(double p) const;
        double log_pdf(double x) const;
        // ostream representation of TruncatedGaussian class
        virtual std::ostream& print(std::ostream& out) const override
        {
            out << "TruncatedGaussian(" << center << "; " << width << "; [" << lower << " , " << upper << "])";
            return out;
        }
};



} // namespace DNest4

#endif

