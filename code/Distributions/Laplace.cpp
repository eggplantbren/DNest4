#include "Laplace.h"
#include <stdexcept>
#include <cmath>
#include "../Utils.h"

namespace DNest4
{

Laplace::Laplace(double center, double width)
:center(center)
,width(width)
{
    if(width <= 0.0)
        throw std::domain_error("Laplace distribution must have positive width.");
}

void Laplace::setpars(double c, double w)
{
    if(w <= 0.0)
        throw std::domain_error("Laplace distribution must have positive width.");
    center = c;
    width = w;
}
double Laplace::cdf(double x) const
{
	return 0.5 + 0.5*sign(x - center)*(1.0 - exp(-std::abs(x - center)/width));
}

double Laplace::cdf_inverse(double x) const
{
    if(x < 0.0 || x > 1.0)
        throw std::domain_error("Input to cdf_inverse must be in [0, 1].");
    return center - width*sign(x - 0.5)*log(1.0 - 2*std::abs(x - 0.5));
}

double Laplace::log_pdf(double x) const
{
    return -log(2.0*width) - std::abs(x - center)/width;
}

int Laplace::sign(double x)
{
	if(x == 0.0)
		return 0;
	if(x > 0.0)
		return 1;
	return -1;
}

} // namespace DNest4

