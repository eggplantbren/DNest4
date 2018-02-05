#include "Exponential.h"
#include <stdexcept>
#include <cmath>
#include <algorithm>    // std::min, std::max
#include <limits>
#include "../Utils.h"

namespace DNest4
{

Exponential::Exponential(double scale)
:scale(scale)
{
    if(scale <= 0.0)
        throw std::domain_error("Exponential distribution must have positive scale.");
}

void Exponential::setpars(double s)
{
    if(s <= 0.0)
        throw std::domain_error("Exponential distribution must have positive scale.");
    scale = s;
}
    
double Exponential::cdf(double x) const
{
    return 1 - exp(-x/scale);
}

double Exponential::cdf_inverse(double p) const
{
    if(p < 0.0 || p > 1.0)
        throw std::domain_error("Input to cdf_inverse must be in [0, 1].");
    return - scale * log(1.-p);
}

double Exponential::log_pdf(double x) const
{
    if(x < 0.0)
        return -std::numeric_limits<double>::infinity();
    return -log(scale) - x/scale;
}


TruncatedExponential::TruncatedExponential(double scale, double lower, double upper)
:scale(scale)
,lower(lower), upper(upper)
{
    if(scale <= 0.0)
        throw std::domain_error("TruncatedExponential distribution must have positive scale.");
    if(lower >= upper)
        throw std::domain_error("TruncatedExponential: lower bound should be less than upper bound.");
    // the original, untruncated, Exponential distribution
    unE = Exponential(scale);
    c = unE.cdf(upper) - unE.cdf(lower);
}

void TruncatedExponential::setpars(double s)
{
    if(s <= 0.0)
        throw std::domain_error("TruncatedExponential distribution must have positive scale.");
    scale = s;
}

double TruncatedExponential::cdf(double x) const
{
    double up = std::max(std::min(x,upper), lower);
    return (unE.cdf(up) - unE.cdf(lower)) / c;
}

double TruncatedExponential::cdf_inverse(double x) const
{
    if(x < 0.0 || x > 1.0)
        throw std::domain_error("Input to cdf_inverse must be in [0, 1].");
    double xx = unE.cdf(lower) + x * c;
    return unE.cdf_inverse(xx);
}

double TruncatedExponential::log_pdf(double x) const
{
    if(x<lower or x>upper) return -std::numeric_limits<double>::infinity();
    return unE.log_pdf(x) - log(c);
}



} // namespace DNest4

