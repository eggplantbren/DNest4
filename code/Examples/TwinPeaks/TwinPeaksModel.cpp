#include "TwinPeaksModel.h"
#include "DNest4/code/Utils.h"

TwinPeaksModel::TwinPeaksModel()
:xs(N)
{

}

void TwinPeaksModel::from_prior(DNest4::RNG& rng)
{
    for(double& x: xs)
        x = rng.rand();
}

double TwinPeaksModel::perturb(DNest4::RNG& rng)
{
    int k = rng.rand_int(N);
    xs[k] += rng.randh2();
    DNest4::wrap(xs[k], 0.0, 1.0);
    return 0.0;
}

void TwinPeaksModel::print(std::ostream& out) const
{
    for(double x: xs)
        out << x << ' ';
}

double TwinPeaksModel::f() const
{
    double result = 0.0;
    for(double x: xs)
        result += -0.5*pow((x - 0.5)/0.01, 2);
    return result;
}

double TwinPeaksModel::g() const
{
    double result = 0.0;
    for(double x: xs)
        result += -0.5*pow((x - 0.45)/0.01, 2);
    return result;
}
