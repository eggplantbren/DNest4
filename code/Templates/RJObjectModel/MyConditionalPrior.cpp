#include "MyConditionalPrior.h"
#include "DNest4/code/DNest4.h"
#include <cmath>

MyConditionalPrior::MyConditionalPrior()
{

}

void MyConditionalPrior::from_prior(DNest4::RNG& rng)
{

}

double MyConditionalPrior::perturb_hyperparameters(DNest4::RNG& rng)
{
    double logH = 0.0;

    return logH;
}

double MyConditionalPrior::log_pdf(const std::vector<double>& vec) const
{
    return 0.0;
}

void MyConditionalPrior::from_uniform(std::vector<double>& vec) const
{

}

void MyConditionalPrior::to_uniform(std::vector<double>& vec) const
{

}

void MyConditionalPrior::print(std::ostream& out) const
{
    out<<' ';
}

