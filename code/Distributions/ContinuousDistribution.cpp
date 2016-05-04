#include "ContinuousDistribution.h"
#include "../Utils.h"

namespace DNest4
{

double ContinuousDistribution::generate(RNG& rng) const
{
    return cdf_inverse(rng.rand());
}

double ContinuousDistribution::perturb(double x, RNG& rng) const
{
    double u = cdf(x);
    u += rng.randh();
    wrap(u, 0.0, 1.0);
    return cdf_inverse(u);
}

} // namespace DNest4

