#include "Rayleigh.h"
#include <stdexcept>
#include <cmath>
#include <limits>
#include "../Utils.h"

namespace DNest4
{

Rayleigh::Rayleigh(double scale)
:scale(scale)
{
    if(scale <= 0.0)
        throw std::domain_error("Rayleigh distribution must have positive scale.");
}

double Rayleigh::cdf(double x) const
{
    return 1 - exp(- (x*x) / (2.*scale*scale));
}

double Rayleigh::cdf_inverse(double x) const
{
    if(x < 0.0 || x > 1.0)
        throw std::domain_error("Input to cdf_inverse must be in [0, 1].");
    return scale * sqrt(-2.*log(1-x));
}

double Rayleigh::log_pdf(double x) const
{
    double r = x/scale;
    return log(r) - 0.5 * r * r;
}



TruncatedRayleigh::TruncatedRayleigh(double scale, double lower, double upper)
:scale(scale),
lower(lower),
upper(upper)
{
    if(scale <= 0.0)
        throw std::domain_error("TruncatedRayleigh distribution must have positive scale.");
    if(lower >= upper)
        throw std::domain_error("TruncatedRayleigh: lower bound should be less than upper bound.");

    lcdf = 1. - exp(- (lower*lower) / (2.*scale*scale));
    ucdf = 1. - exp(- (upper*upper) / (2.*scale*scale));
    tp = ucdf - lcdf;
    logtp = log(tp);
}

double TruncatedRayleigh::cdf(double x) const
{
    double cdfr = 1 - exp(- (x*x) / (2.*scale*scale)); // untruncated cdf
    return (cdfr - lcdf) / tp;
}

double TruncatedRayleigh::cdf_inverse(double x) const
{
    if(x < 0.0 || x > 1.0)
        throw std::domain_error("Input to cdf_inverse must be in [0, 1].");
    double u = lcdf + x*tp;
    return scale * sqrt(-2.*log(1-u));
}

double TruncatedRayleigh::log_pdf(double x) const
{
    if(x<lower or x>=upper) return -std::numeric_limits<double>::infinity();
    double r = x/scale;
    return log(r) - 0.5 * r * r - logtp;
}

} // namespace DNest4

