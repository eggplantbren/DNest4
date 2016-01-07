#include "MyDistribution.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <cmath>

using namespace DNest3;

MyDistribution::MyDistribution(double x_min, double x_max,
					double mu_min, double mu_max)
:x_min(x_min)
,x_max(x_max)
,mu_min(mu_min)
,mu_max(mu_max)
{

}

void MyDistribution::fromPrior()
{
	mu = exp(log(mu_min) + log(mu_max/mu_min)*randomU());
}

double MyDistribution::perturb_parameters()
{
	double logH = 0.;

	mu = log(mu);
	mu += log(mu_max/mu_min)*pow(10., 1.5 - 6.*randomU())*randn();
	mu = mod(mu - log(mu_min), log(mu_max/mu_min)) + log(mu_min);
	mu = exp(mu);

	return logH;
}

// vec[0] = "position" (log-period)
// vec[1] = amplitude
// vec[2] = phase

double MyDistribution::log_pdf(const std::vector<double>& vec) const
{
	if(vec[0] < x_min || vec[0] > x_max || vec[1] < 0. ||
			vec[2] < 0. || vec[2] > 2.*M_PI)
		return -1E300;

	return -log(mu) - vec[1]/mu;
}

void MyDistribution::from_uniform(std::vector<double>& vec) const
{
	vec[0] = x_min + (x_max - x_min)*vec[0];
	vec[1] = -mu*log(1. - vec[1]);
	vec[2] = 2.*M_PI*vec[2];
}

void MyDistribution::to_uniform(std::vector<double>& vec) const
{
	vec[0] = (vec[0] - x_min)/(x_max - x_min);
	vec[1] = 1. - exp(-vec[1]/mu);
	vec[2] = vec[2]/(2.*M_PI);
}

void MyDistribution::print(std::ostream& out) const
{
	out<<mu<<' ';
}

