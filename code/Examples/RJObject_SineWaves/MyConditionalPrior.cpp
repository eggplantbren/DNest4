#include "MyConditionalPrior.h"
#include "Utils.h"
#include <cmath>

using namespace DNest4;

MyConditionalPrior::MyConditionalPrior(double x_min, double x_max,
					double mu_min, double mu_max)
:x_min(x_min)
,x_max(x_max)
,mu_min(mu_min)
,mu_max(mu_max)
{

}

void MyConditionalPrior::from_prior(RNG& rng)
{
	mu = exp(log(mu_min) + log(mu_max/mu_min)*rng.rand());
}

double MyConditionalPrior::perturb_parameters(RNG& rng)
{
	double logH = 0.;

	mu = log(mu);
	mu += log(mu_max/mu_min)*rng.randh();
	mu = mod(mu - log(mu_min), log(mu_max/mu_min)) + log(mu_min);
	mu = exp(mu);

	return logH;
}

// vec[0] = "position" (log-period)
// vec[1] = amplitude
// vec[2] = phase

double MyConditionalPrior::log_pdf(const std::vector<double>& vec) const
{
	if(vec[0] < x_min || vec[0] > x_max || vec[1] < 0. ||
			vec[2] < 0. || vec[2] > 2.*M_PI)
		return -1E300;

	return -log(mu) - vec[1]/mu;
}

void MyConditionalPrior::from_uniform(std::vector<double>& vec) const
{
	vec[0] = x_min + (x_max - x_min)*vec[0];
	vec[1] = -mu*log(1. - vec[1]);
	vec[2] = 2.*M_PI*vec[2];
}

void MyConditionalPrior::to_uniform(std::vector<double>& vec) const
{
	vec[0] = (vec[0] - x_min)/(x_max - x_min);
	vec[1] = 1. - exp(-vec[1]/mu);
	vec[2] = vec[2]/(2.*M_PI);
}

void MyConditionalPrior::print(std::ostream& out) const
{
	out<<mu<<' ';
}

