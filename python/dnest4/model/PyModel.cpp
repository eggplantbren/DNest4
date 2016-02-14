#include "PyModel.h"

using namespace std;
using namespace DNest4;

/* PyModel::PyModel() */
/* { */
/*     sigma = 1.0; */
/* } */

// void PyModel::from_prior(RNG& rng)
// {
//     // Naive diffuse prior
//     m = 10*rng.randn();
// }

// double PyModel::perturb(RNG& rng)
// {
//     double log_H = 0.;
//
//     log_H -= -0.5*pow(m/10, 2);
//     m += 10*rng.randh();
//     log_H += -0.5*pow(m/10, 2);
//
//     return log_H;
// }

// double PyModel::log_likelihood() const
// {
//     // Variance
//     double var = sigma*sigma;
//
//     // Conventional gaussian sampling distribution
//     return -0.5*log(2*M_PI*var) - 0.5 * m*m / var;
// }

// void PyModel::print(std::ostream& out) const
// {
//     out<<m;
// }
//
// string PyModel::description() const
// {
//     return string("m, sigma");
// }

