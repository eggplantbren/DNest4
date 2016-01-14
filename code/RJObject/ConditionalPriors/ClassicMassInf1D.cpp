#include "ClassicMassInf1D.h"
#include "../../Utils.h"
#include <cmath>

using namespace DNest4;

ClassicMassInf1D::ClassicMassInf1D(double x_min, double x_max,
					double mu_min, double mu_max)
:x_min(x_min)
,x_max(x_max)
,mu_min(mu_min)
,mu_max(mu_max)
{

}

void ClassicMassInf1D::from_prior(RNG& rng)
{
	mu = exp(log(mu_min) + log(mu_max/mu_min)*rng.rand());
}

double ClassicMassInf1D::perturb_hyperparameters(RNG& rng)
{
	double logH = 0.;

	mu = log(mu);
	mu += log(mu_max/mu_min)*rng.randh();
	mu = mod(mu - log(mu_min), log(mu_max/mu_min)) + log(mu_min);
	mu = exp(mu);

	return logH;
}

double ClassicMassInf1D::log_pdf(const std::vector<double>& vec) const
{
	if(vec[0] < x_min || vec[0] > x_max || vec[1] < 0.)
		return -1E300;

	return -log(mu) - vec[1]/mu;
}

void ClassicMassInf1D::from_uniform(std::vector<double>& vec) const
{
	vec[0] = x_min + (x_max - x_min)*vec[0];
	vec[1] = -mu*log(1. - vec[1]);
}

void ClassicMassInf1D::to_uniform(std::vector<double>& vec) const
{
	vec[0] = (vec[0] - x_min)/(x_max - x_min);
	vec[1] = 1. - exp(-vec[1]/mu);
}

void ClassicMassInf1D::print(std::ostream& out) const
{
	out<<mu<<' ';
}

