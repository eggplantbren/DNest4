#include "Triangular.h"
#include "../RNG.h"
#include <stdexcept>
#include <cmath>
#include <limits>
#include "../Utils.h"

namespace DNest4
{

Triangular::Triangular(double lower, double centre, double upper)
:lower(lower)
,centre(centre)
,upper(upper)
{
    if (upper < centre)
        throw std::domain_error("Triangular distribution must have upper >= centre limits");

    if (lower > centre)
        throw std::domain_error("Triangular distribution must have lower <= centre limits");

    if (upper <= lower)
        throw std::domain_error("Triangular distribution must have lower < upper limits");
}

double Triangular::cdf(double x) const
{
    if (x<=lower)
        return 0.;
    else if ((x>lower) && (x<=centre))
        return pow(x-lower, 2)/((upper-lower)*(centre-lower));
    else if ((x>centre) && (x<upper))
        return 1. - pow(upper-x, 2)/((upper-lower)*(upper-centre));
    else
        return 1.;
}

double Triangular::cdf_inverse(double x) const
{
    const double centre_ccdf = (centre-lower)/(upper-lower);
    if(x < 0. || x > 1.)
        throw std::domain_error("Input to cdf_inverse must be in [0, 1].");
    else if (x <= centre_ccdf)
        return lower + sqrt((upper-lower)*(centre-lower)*x);
    else
        return upper - sqrt((1.-x)*(upper-lower)*(upper-centre));
}

double Triangular::log_pdf(double x) const
{
    if (x<lower or x>upper)
        return -std::numeric_limits<double>::infinity();
    else if ((x>lower) && (x<=centre))
        return log(2.0*(x-lower)) - log((upper-lower)*(centre-lower));
    else
        return log((upper-lower)) - log((upper-lower)*(upper-centre));
}

} // namespace DNest4
