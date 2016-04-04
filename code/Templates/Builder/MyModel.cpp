#include "MyModel.h"
#include "DNest4/code/DNest4.h"

using namespace std;
using namespace DNest4;

const std::vector<double> MyModel::y{1.01, 1.99, 3.02, 3.88, 5.01};
const std::vector<double> MyModel::x{1.0, 2.0, 3.0, 4.0, 5.0};
const int MyModel::N{5};


MyModel::MyModel()

{

}

void MyModel::from_prior(RNG& rng)
{
    m = 0.0 + 1000.0*rng.randn();
    b = 0.0 + 1000.0*rng.randn();
    sigma = exp(log(0.001) + log((1000.0)/(0.001))*rng.rand());
}

double MyModel::perturb(RNG& rng)
{
    double log_H = 0.0;
    int which = rng.rand_int(3);
    if(which == 0)
    {
        log_H -= -0.5*pow(((m) - (0.0))/(1000.0), 2);
        m += (1000.0)*rng.randh();
        log_H += -0.5*pow(((m) - (0.0))/(1000.0), 2);
    }
    if(which == 1)
    {
        log_H -= -0.5*pow(((b) - (0.0))/(1000.0), 2);
        b += (1000.0)*rng.randh();
        log_H += -0.5*pow(((b) - (0.0))/(1000.0), 2);
    }
    if(which == 2)
    {
        sigma = log(sigma);
        sigma += log((1000.0)/(0.001))*rng.randh();
        wrap(sigma, log(0.001), log(1000.0));
        sigma = exp(sigma);
    }
    return log_H;
}

double MyModel::log_likelihood() const
{
    double logp = 0.0;
    
    logp += -0.5*log(2*M_PI) - log(sigma) - 0.5*pow(((y[0]) - (m*x[0] + b))/(sigma), 2);
    logp += -0.5*log(2*M_PI) - log(sigma) - 0.5*pow(((y[1]) - (m*x[1] + b))/(sigma), 2);
    logp += -0.5*log(2*M_PI) - log(sigma) - 0.5*pow(((y[2]) - (m*x[2] + b))/(sigma), 2);
    logp += -0.5*log(2*M_PI) - log(sigma) - 0.5*pow(((y[3]) - (m*x[3] + b))/(sigma), 2);
    logp += -0.5*log(2*M_PI) - log(sigma) - 0.5*pow(((y[4]) - (m*x[4] + b))/(sigma), 2);
    
    return logp;
}

void MyModel::print(std::ostream& out) const
{
    out<<m<<" ";
    out<<b<<" ";
    out<<sigma<<" ";
}

string MyModel::description() const
{
    string s;
    s += "m, ";
    s += "b, ";
    s += "sigma";
    return s;
}

