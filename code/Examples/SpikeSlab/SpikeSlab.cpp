#include "SpikeSlab.h"
#include "DNest4/code/DNest4.h"

using namespace std;
using namespace DNest4;

SpikeSlab::SpikeSlab()
:params(20)
{

}

void SpikeSlab::from_prior(RNG& rng)
{
	for(size_t i=0; i<params.size(); i++)
		params[i] = -0.5 + rng.rand();
}

double SpikeSlab::perturb(RNG& rng)
{
	int which = rng.rand_int(params.size());
	params[which] += rng.randh();
	wrap(params[which], -0.5, 0.5);
	return 0.;
}

double SpikeSlab::log_likelihood() const
{
	constexpr double u = 0.01;
	constexpr double v = 0.1;
	const double C = log(1.0/sqrt(2*M_PI));

	double logl1 = params.size()*(C - log(u));
	double logl2 = params.size()*(C - log(v));

	for(const double& x: params)
	{
		logl1 += -0.5*pow((x - 0.031)/u, 2);
		logl2 += -0.5*pow(x/v, 2);
	}
	logl1 += log(100.);

	return logsumexp(logl1, logl2);
}

void SpikeSlab::print(std::ostream& out) const
{
	for(const double& x: params)
		out<<x<<' ';
}

string SpikeSlab::description() const
{
	return string("Each column is one of the 20 parameters.");
}

