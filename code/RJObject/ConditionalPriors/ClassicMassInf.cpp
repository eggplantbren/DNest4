#include "ClassicMassInf.h"
#include "../../Utils.h"
#include <cmath>

using namespace DNest4;

ClassicMassInf::ClassicMassInf(double x_min, double x_max,
					double y_min, double y_max,
					double mu_min, double mu_max)
:x_min(x_min)
,x_max(x_max)
,y_min(y_min)
,y_max(y_max)
,mu_min(mu_min)
,mu_max(mu_max)
{

}

void ClassicMassInf::from_prior(RNG& rng)
{
	mu = exp(log(mu_min) + log(mu_max/mu_min)*rng.rand());
}

double ClassicMassInf::perturb_hyperparameters(RNG& rng)
{
	double logH = 0.;

	mu = log(mu);
	mu += log(mu_max/mu_min)*rng.randh();
	mu = mod(mu - log(mu_min), log(mu_max/mu_min)) + log(mu_min);
	mu = exp(mu);

	return logH;
}

double ClassicMassInf::log_pdf(const std::vector<double>& vec) const
{
	if(vec[0] < x_min || vec[0] > x_max ||
		vec[1] < y_min || vec[1] > y_max ||
		vec[2] < 0.)
		return -1E300;

	return -log(mu) - vec[2]/mu;
}

void ClassicMassInf::from_uniform(std::vector<double>& vec) const
{
	vec[0] = x_min + (x_max - x_min)*vec[0];
	vec[1] = y_min + (y_max - y_min)*vec[1];
	vec[2] = -mu*log(1. - vec[2]);
}

void ClassicMassInf::to_uniform(std::vector<double>& vec) const
{
	vec[0] = (vec[0] - x_min)/(x_max - x_min);
	vec[1] = (vec[1] - y_min)/(y_max - y_min);
	vec[2] = 1. - exp(-vec[2]/mu);
}

void ClassicMassInf::print(std::ostream& out) const
{
	out<<mu<<' ';
}

