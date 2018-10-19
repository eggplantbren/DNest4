#include "Fixed.h"
#include <stdexcept>
#include <cmath>
#include <limits>
#include "../Utils.h"

namespace DNest4
{

Fixed::Fixed(double val)
:val(val)
    {
        // if(a <= 0.0 or b <= 0.0)
            // throw std::domain_error("Fixed distribution must have positive a and b.");
    }

    double Fixed::cdf(double x) const
    {
        if(x == val)
            return 1.0;
        else
            return 0.0;
    }

    double Fixed::cdf_inverse(double p) const
    {
        if(p < 0.0 || p > 1.0)
            throw std::domain_error("Input to cdf_inverse must be in [0, 1].");
        return val;
    }

    double Fixed::log_pdf(double x) const
    {
        if(x == val)
            return 0.0;
        else
            return -std::numeric_limits<double>::infinity();;
    }

    double Fixed::perturb(double& x, RNG& rng) const
    // in this class, perturb does nothing
    {
        (void)x; (void)rng; // to silence the unused parameter warnings
        return 0.0;
    }



} // namespace DNest4

