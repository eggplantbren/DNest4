#include "Jeffreys.h"
#include <stdexcept>
#include <cmath>
#include "../Utils.h"

namespace DNest4
{

Jeffreys::Jeffreys(double lower, double upper)
:lower(lower)
,upper(upper)
{
    if(lower <= 0.0)
        throw std::domain_error("Jeffreys distribution must have positive lower limit.");
    if (upper <= lower)
        throw std::domain_error("Jeffreys distribution must have lower < upper limits");

    d = log(upper / lower);
}

double Jeffreys::cdf(double x) const
{
    return (log(x) - log(lower)) / d;
}

double Jeffreys::cdf_inverse(double x) const
{
    if(x < 0.0 || x > 1.0)
        throw std::domain_error("Input to cdf_inverse must be in [0, 1].");
    return lower * pow(upper/lower, x);
}

double Jeffreys::log_pdf(double x) const
{
    return -log(x) - log(d);
}


ModifiedJeffreys::ModifiedJeffreys(double knee, double upper)
:knee(knee)
,upper(upper)
{
    if(knee <= 0.0)
        throw std::domain_error("ModifiedJeffreys distribution must have positive knee.");
    if (upper <= knee)
        throw std::domain_error("ModifiedJeffreys distribution must have knee < upper limit");

    d = 1.0 / log(1.0 + upper / knee);
}

double ModifiedJeffreys::cdf(double x) const
{
    return d * log(1.0 + x / knee);
}

double ModifiedJeffreys::cdf_inverse(double x) const
{
    if(x < 0.0 || x > 1.0)
        throw std::domain_error("Input to cdf_inverse must be in [0, 1].");
    return (exp(x/d) - 1.0) * knee;
}

double ModifiedJeffreys::log_pdf(double x) const
{
    return log(d) - log(1.0 + x/knee);
}


} // namespace DNest4

