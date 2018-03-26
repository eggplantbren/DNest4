#include "Data.h"
#include "StraightLine.h"
#include "DNest4/code/DNest4.h"

using namespace std;
using namespace DNest4;

StraightLine::StraightLine()
{

}

void StraightLine::calculate_mu()
{
	const auto& x = Data::get_instance().get_x();
	mu = m*x + b;
}

void StraightLine::from_prior(RNG& rng)
{
	// Naive diffuse prior
	m = 1E3*rng.randn();
	b = 1E3*rng.randn();

	// Log-uniform prior
	sigma = exp(-10. + 20.*rng.rand());

	// Compute the model line
	calculate_mu();
}

double StraightLine::perturb(RNG& rng)
{
	double log_H = 0.;

	// Proposals explore the prior
	// For normal priors I usually use the hastings factor to do it
	int which = rng.rand_int(3);
	if(which == 0)
	{
		log_H -= -0.5*pow(m/1E3, 2);
		m += 1E3*rng.randh();
		log_H += -0.5*pow(m/1E3, 2);
	}
	else if(which == 1)
	{
		log_H -= -0.5*pow(b/1E3, 2);
		b += 1E3*rng.randh();
		log_H += -0.5*pow(b/1E3, 2);
	}
	else
	{
		// Usual log-uniform prior trick
		sigma = log(sigma);
		sigma += 20.*rng.randh();
		wrap(sigma, -10., 10.);
		sigma = exp(sigma);
	}

	// Pre-reject
	if(rng.rand() >= exp(log_H))
		return -1E300;
	else
		log_H = 0.0;

	// Calculate mu again if m or b changed
	if(which == 0 || which == 1)
		calculate_mu();

	return log_H;
}

double StraightLine::log_likelihood() const
{
	// Grab the y-values from the dataset
	const auto& y = Data::get_instance().get_y();

	// Variance
	double var = sigma*sigma;

	// Conventional gaussian sampling distribution
	return -0.5*y.size()*log(2*M_PI*var) - 0.5*pow(y - mu, 2).sum()/var;
}

void StraightLine::print(std::ostream& out) const
{
	out<<m<<' '<<b<<' '<<sigma;
}

string StraightLine::description() const
{
	return string("m, b, sigma");
}

