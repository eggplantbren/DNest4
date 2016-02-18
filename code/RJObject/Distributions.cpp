#include <cassert>
#include <cmath>
#include "Distributions.h"

namespace DNest4
{

int sign(double x)
{
	if(x == 0.)
		return 0;
	if(x > 0.)
		return 1;
	return -1;
}

double laplacian_cdf(double x, double center, double width)
{
	assert(width > 0.);
	return 0.5 + 0.5*sign(x - center)*(1. - exp(-std::abs(x - center)/width));
}

double laplacian_cdf_inverse(double x, double center,
														double width)
{
	assert(width > 0.);
	assert(x >= 0. && x <= 1.);

	return center - width*sign(x - 0.5)*log(1. - 2.*std::abs(x - 0.5));
}

} // namespace DNest4

