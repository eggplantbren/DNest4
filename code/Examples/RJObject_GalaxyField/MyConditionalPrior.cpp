#include "MyConditionalPrior.h"
#include "DNest4/code/DNest4.h"
#include <cmath>

using namespace DNest4;

MyConditionalPrior::MyConditionalPrior(double x_min, double x_max,
					double y_min, double y_max,
					double fluxlim_min, double fluxlim_max)
:x_min(x_min)
,x_max(x_max)
,y_min(y_min)
,y_max(y_max)
,fluxlim_min(fluxlim_min)
,fluxlim_max(fluxlim_max)
,radiuslim_min(1E-3*sqrt((x_max - x_min)*(y_max - y_min)))
,radiuslim_max(sqrt((x_max - x_min)*(y_max - y_min)))
{

}

void MyConditionalPrior::from_prior(RNG& rng)
{
	fluxlim = exp(log(fluxlim_min) + log(fluxlim_max/fluxlim_min)*rng.rand());
	gamma = rng.rand();

	radiuslim = exp(log(radiuslim_min) + log(radiuslim_max/radiuslim_min)*rng.rand());
	gamma_radius = rng.rand();

	b1 = rng.rand();
	a1 = rng.rand()*b1;

	b2 = rng.rand();
	a2 = rng.rand()*b2;
}

double MyConditionalPrior::perturb_hyperparameters(RNG& rng)
{
	double logH = 0.;

	int which = rng.rand_int(8);

	if(which == 0)
	{
		fluxlim = log(fluxlim);
		fluxlim += log(fluxlim_max/fluxlim_min)*rng.randh();
		fluxlim = mod(fluxlim - log(fluxlim_min),
			log(fluxlim_max/fluxlim_min)) + log(fluxlim_min);
		fluxlim = exp(fluxlim);
	}
	else if(which == 1)
	{
		gamma += rng.randh();
		gamma = mod(gamma, 1.);
	}
	else if(which == 2)
	{
		radiuslim = log(radiuslim);
		radiuslim += log(radiuslim_max/radiuslim_min)*rng.randh();
		radiuslim = mod(radiuslim - log(radiuslim_min),
			log(radiuslim_max/radiuslim_min)) + log(radiuslim_min);
		radiuslim = exp(radiuslim);
	}
	else if(which == 3)
	{
		gamma_radius += rng.randh();
		gamma_radius = mod(gamma_radius, 1.);
	}
	else if(which == 4)
	{
		a1 /= b1;
		b1 += rng.randh();
		b1 = mod(b1, 1.);
		a1 *= b1;
	}
	else if(which == 5)
	{
		a1 += b1*rng.randh();
		a1 = mod(a1, b1);
	}
	else if(which == 6)
	{
		a2 /= b2;
		b2 += rng.randh();
		b2 = mod(b2, 1.);
		a2 *= b2;
	}
	else
	{
		a2 += b2*rng.randh();
		a2 = mod(a2, b2);
	}

	return logH;
}

// x, y, flux, radius, q, theta, rinner/router, Minner/Mouter
double MyConditionalPrior::log_pdf(const std::vector<double>& vec) const
{
	double alpha = 1./gamma;
	double alpha_radius = 1./gamma_radius;

	if(vec[0] < x_min || vec[0] > x_max ||
			vec[1] < y_min || vec[1] > y_max ||
			vec[2] < fluxlim || vec[3] < radiuslim ||
			vec[4] < 0.2 || vec[4] > 1. ||
			vec[5] < 0. || vec[5] > M_PI ||
			vec[6] < a1 || vec[6] > b1 ||
			vec[7] < a2 || vec[7] > b2)
		return -1E300;

	double logp = 0.;

	logp += log(alpha) + alpha*log(fluxlim) - (alpha + 1.)*log(vec[2]);
	logp += log(alpha_radius) + alpha_radius*log(radiuslim)
			- (alpha_radius + 1.)*log(vec[3]);
	logp += -log(b1 - a1) - log(b2 - a2);

	return logp;
}

void MyConditionalPrior::from_uniform(std::vector<double>& vec) const
{
	vec[0] = x_min + (x_max - x_min)*vec[0];
	vec[1] = y_min + (y_max - y_min)*vec[1];
	vec[2] = fluxlim*pow(1. - vec[2], -gamma);
	vec[3] = radiuslim*pow(1. - vec[3], -gamma_radius);
	vec[4] = 0.2 + 0.8*vec[4];
	vec[5] = M_PI*vec[5];
	vec[6] = a1 + (b1 - a1)*vec[6];
	vec[7] = a2 + (b2 - a2)*vec[7];
}

void MyConditionalPrior::to_uniform(std::vector<double>& vec) const
{
	double alpha = 1./gamma;
	double alpha_radius = 1./gamma_radius;

	vec[0] = (vec[0] - x_min)/(x_max - x_min);
	vec[1] = (vec[1] - y_min)/(y_max - y_min);
	vec[2] = 1. - pow(fluxlim/vec[2], alpha);
	vec[3] = 1. - pow(radiuslim/vec[3], alpha_radius);
	vec[4] = (vec[4] - 0.2)/0.8;
	vec[5] = vec[5]/M_PI;
	vec[6] = (vec[6] - a1)/(b1 - a1);
	vec[7] = (vec[7] - a2)/(b2 - a2);
}

void MyConditionalPrior::print(std::ostream& out) const
{
	out<<fluxlim<<' '<<gamma<<' '<<radiuslim<<' '<<gamma_radius<<' ';
	out<<a1<<' '<<b1<<' '<<a2<<' '<<b2<<' ';
}

