#include "Cauchy.h"
#include <stdexcept>
#include <cmath>
#include <limits>
#include "../Utils.h"

namespace DNest4
{

Cauchy::Cauchy(double center, double width)
:center(center)
,width(width)
{
    if(width <= 0.0)
        throw std::domain_error("Cauchy distribution must have positive width.");
}

double Cauchy::cdf(double x) const
{
    return atan((x - center)/width)/M_PI + 0.5;
}

double Cauchy::cdf_inverse(double x) const
{
    if(x < 0.0 || x > 1.0)
        throw std::domain_error("Input to cdf_inverse must be in [0, 1].");
    return center + width*tan(M_PI*(x - 0.5));
}

double Cauchy::log_pdf(double x) const
{
    return -log(M_PI*width) - log(1.0 + pow((x - center)/width, 2));
}




TruncatedCauchy::TruncatedCauchy(double center, double width,
                                 double lower, double upper)
:center(center), width(width)
,lower(lower), upper(upper)
{
    if(width <= 0.0)
        throw std::domain_error("TruncatedCauchy distribution must have positive width.");
    if(lower >= upper)
        throw std::domain_error("TruncatedCauchy: lower bound should be less than upper bound.");
    // the original, untruncated, Cauchy distribution
    unC = Cauchy(center, width);
    c = unC.cdf(upper) - unC.cdf(lower);
}

double TruncatedCauchy::cdf(double x) const
{
    double up = std::max(std::min(x,upper), lower);
    return (unC.cdf(up) - unC.cdf(lower)) / c;
}

double TruncatedCauchy::cdf_inverse(double x) const
{
    if(x < 0.0 || x > 1.0)
        throw std::domain_error("Input to cdf_inverse must be in [0, 1].");
    double xx = unC.cdf(lower) + x * c;
    return unC.cdf_inverse(xx);
}

double TruncatedCauchy::log_pdf(double x) const
{
    if(x<lower or x>upper) return -std::numeric_limits<double>::infinity();
    return unC.log_pdf(x) - log(c);
}


} // namespace DNest4

