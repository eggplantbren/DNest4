#include "MyModel.h"
#include "DNest4/code/DNest4.h"

MyModel::MyModel()
:xs(10)
{

}

void MyModel::from_prior(DNest4::RNG& rng)
{
    for(double& x: xs)
        x = -5.12 + 10.24*rng.rand();
}

double MyModel::perturb(DNest4::RNG& rng)
{
    double logH = 0.0;

    int which = rng.rand_int(xs.size());
    xs[which] += 10.24*rng.randh();
    DNest4::wrap(xs[which], -5.12, 5.12);

    return logH;
}

double MyModel::log_likelihood() const
{
    double logL = 0.0;

    // Based on Rastrigin function

    static constexpr double A = 10.0;
    size_t n = xs.size();
    for(double x: xs)
        logL -= x*x - A*cos(2*M_PI*x);
    logL -= A*n;

    return logL;
}

void MyModel::print(std::ostream& out) const
{
    for(size_t i=0; i<xs.size(); ++i)
        out << xs[i] << ' ';

}

std::string MyModel::description() const
{
    return std::string("");
}

