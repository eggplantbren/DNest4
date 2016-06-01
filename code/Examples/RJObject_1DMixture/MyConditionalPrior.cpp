#include "MyConditionalPrior.h"
#include "DNest4/code/DNest4.h"
#include <cmath>

using namespace DNest4;

MyConditionalPrior::MyConditionalPrior()
{

}

void MyConditionalPrior::from_prior(RNG& rng)
{
    location_mu = -1000.0 + 2000.0*rng.rand();
    scale_mu = exp(-10.0 + 20.0*rng.rand());

    location_log_sigma = -10.0 + 20.0*rng.rand();
    scale_log_sigma = 5.0*rng.rand();

    scale_log_weight = 5.0*rng.rand();
}

double MyConditionalPrior::perturb_hyperparameters(RNG& rng)
{
	double logH = 0.0;

    // Choose a parameter to move
    int which = rng.rand_int(5);

    if(which == 0)
    {
        location_mu += 2E3*rng.randh();
        wrap(location_mu, -1000.0, 1000.0);
    }
    else if(which == 1)
    {
        scale_mu = log(scale_mu);
        scale_mu += 20.0*rng.randh();
        wrap(scale_mu, -10.0, 10.0);
        scale_mu = exp(scale_mu);
    }
    else if(which == 2)
    {
        location_log_sigma += 20.0*rng.randh();
        wrap(location_log_sigma, -10.0, 10.0);
    }
    else if(which == 3)
    {
        scale_log_sigma += 5.0*rng.randh();
        wrap(scale_log_sigma, 0.0, 5.0);
    }
    else
    {
        scale_log_weight += 5.0*rng.randh();
        wrap(scale_log_weight, 0.0, 5.0);
    }

	return logH;
}

// vec = {mu, log_sigma, log_weight}

double MyConditionalPrior::log_pdf(const std::vector<double>& vec) const
{
    // Three Laplace distributions
    Laplace l1(location_mu, scale_mu);
    Laplace l2(location_log_sigma, scale_log_sigma);
    Laplace l3(0.0, scale_log_weight);

	return l1.log_pdf(vec[0]) + l2.log_pdf(vec[1]) + l3.log_pdf(vec[2]);
}

void MyConditionalPrior::from_uniform(std::vector<double>& vec) const
{
    // Three Laplace distributions
    Laplace l1(location_mu, scale_mu);
    Laplace l2(location_log_sigma, scale_log_sigma);
    Laplace l3(0.0, scale_log_weight);

    vec[0] = l1.cdf_inverse(vec[0]);
    vec[1] = l2.cdf_inverse(vec[1]);
    vec[2] = l3.cdf_inverse(vec[2]);
}

void MyConditionalPrior::to_uniform(std::vector<double>& vec) const
{
    // Three Laplace distributions
    Laplace l1(location_mu, scale_mu);
    Laplace l2(location_log_sigma, scale_log_sigma);
    Laplace l3(0.0, scale_log_weight);

    vec[0] = l1.cdf(vec[0]);
    vec[1] = l2.cdf(vec[1]);
    vec[2] = l3.cdf(vec[2]);
}

void MyConditionalPrior::print(std::ostream& out) const
{
    out<<location_mu<<' '<<scale_mu<<' '<<location_log_sigma<<' ';
    out<<scale_log_sigma<<' '<<scale_log_weight<<' ';
}

