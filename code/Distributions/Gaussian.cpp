#include "Gaussian.h"
#include <iostream>
#include <stdexcept>
#include <cmath>
#include "../Utils.h"
//#include <boost/math/special_functions/erf.hpp>

namespace DNest4
{

Gaussian::Gaussian(double center, double width)
:center(center)
,width(width)
{
    if(width <= 0.0)
        throw std::domain_error("Gaussian distribution must have positive width.");
}

double Gaussian::cdf(double x) const
{
    return normal_cdf((x-center)/width);
}

double Gaussian::cdf_inverse(double x) const
{
    if(x < 0.0 || x > 1.0)
        throw std::domain_error("Input to cdf_inverse must be in [0, 1].");
    return center + width*normal_inverse_cdf(x);
    //return center + width * sqrt(2) * boost::math::erf_inv(2*x - 1);
}

double Gaussian::log_pdf(double x) const
{
	double r = (x - center)/width;
    return -0.5*r*r - _norm_pdf_logC;
}


} // namespace DNest4