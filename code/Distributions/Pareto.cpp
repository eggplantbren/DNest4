#include "Pareto.h"
#include <stdexcept>
#include <cmath>
#include "../Utils.h"

namespace DNest4
{

Pareto::Pareto(double _min, double _alpha)
:min(_min)
,alpha(_alpha)
{
    if(min <= 0.0 || alpha <= 0.0)
        throw std::domain_error("Invalid Pareto distribution.");
}

double Pareto::cdf(double x) const
{
    if(x <= min)
        return 0.0;

    return 1.0 - pow(x/min, alpha);
}

double Pareto::cdf_inverse(double x) const
{
    if(x < 0.0 || x > 1.0)
        throw std::domain_error("Input to cdf_inverse must be in [0, 1].");
    return min*pow(1.0 - x, 1.0/alpha);
}

double Pareto::log_pdf(double x) const
{
    if(x <= min)
        return -1E300;
    return log(alpha) + alpha*log(min) - (alpha + 1.0)*log(x);
}

} // namespace DNest4

