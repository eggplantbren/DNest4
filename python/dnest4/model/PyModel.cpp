#include "PyModel.h"

using namespace std;
using namespace DNest4;

PyModel::PyModel()
{
    sigma = 1.0;
}

void PyModel::from_prior(RNG& rng)
{
    // Naive diffuse prior
    m = rng.randn();

    // Log-uniform prior
    sigma = 1.0;  // exp(-10. + 20.*rng.rand());
}

double PyModel::perturb(RNG& rng)
{
    double log_H = 0.;

    // Proposals explore the prior
    // For normal priors I usually use the hastings factor to do it
    // int which = rng.rand_int(2);
    // if(which == 0)
    // {
        log_H -= -0.5*pow(m/1E3, 2);
        m += 1E3*rng.randh();
        log_H += -0.5*pow(m/1E3, 2);
    // }
    // else
    // {
    //     // Usual log-uniform prior trick
    //     sigma = log(sigma);
    //     sigma += 20.*rng.randh();
    //     wrap(sigma, -10., 10.);
    //     sigma = exp(sigma);
    // }

    return log_H;
}

double PyModel::log_likelihood() const
{
    // Variance
    double var = sigma*sigma;

    // Conventional gaussian sampling distribution
    return -0.5*log(2*M_PI*var) - 0.5 * m*m / var;
}

void PyModel::print(std::ostream& out) const
{
    out<<m<<' '<<sigma;
}

string PyModel::description() const
{
    return string("m, sigma");
}

