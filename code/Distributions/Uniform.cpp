#include "Uniform.h"
#include "../RNG.h"
#include <stdexcept>
#include <cmath>
#include <limits>
#include "../Utils.h"

namespace DNest4
{

Uniform::Uniform(double lower, double upper)
:lower(lower)
,upper(upper)
{
    if (upper <= lower)
        throw std::domain_error("Uniform distribution must have lower < upper limits");
}

double Uniform::cdf(double x) const
{
    if (x<lower)
        return 0.;
    else if (x>=upper)
        return 1.;
    else
        return (x-lower)/(upper-lower);
}

double Uniform::cdf_inverse(double x) const
{
    if(x < 0.0 || x > 1.0)
        throw std::domain_error("Input to cdf_inverse must be in [0, 1].");
    return lower + x*(upper-lower);
}

double Uniform::log_pdf(double x) const
{
    if (x<lower or x>upper) return -std::numeric_limits<double>::infinity();
    return -log(upper-lower);
}

} // namespace DNest4

