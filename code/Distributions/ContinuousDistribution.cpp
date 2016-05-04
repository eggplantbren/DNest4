#include "ContinuousDistribution.h"
#include "../Utils.h"

namespace DNest4
{

double ContinuousDistribution::generate(RNG& rng) const
{
    return cdf_inverse(rng.rand());
}

double ContinuousDistribution::perturb(double& x, RNG& rng) const
{
    x = cdf(x);
    x += rng.randh();
    wrap(x, 0.0, 1.0);
    x = cdf_inverse(x);

    return 0.0;
}

} // namespace DNest4

