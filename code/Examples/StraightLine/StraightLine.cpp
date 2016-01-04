#include "StraightLine.h"
#include "Utils.h"

using namespace std;
using namespace DNest4;

StraightLine::StraightLine()
:mu()
{

}

void StraightLine::from_prior(RNG& rng)
{
//	mu = 
//	b = 
}

double StraightLine::perturb(RNG& rng)
{
//	int which = rng.rand_int(params.size());
//	params[which] += rng.randh();
//	wrap(params[which], -0.5, 0.5);
	return 0.;
}

double StraightLine::log_likelihood() const
{
	return 0.;
//	constexpr double u = 0.01;
//	constexpr double v = 0.1;
//	constexpr double C = log(1.0/sqrt(2*M_PI));

//	double logl1 = params.size()*(C - log(u));
//	double logl2 = params.size()*(C - log(v));

//	for(const double& x: params)
//	{
//		logl1 += -0.5*pow((x - 0.031)/u, 2);
//		logl2 += -0.5*pow(x/v, 2);
//	}
//	logl1 += log(100.);

//	return logsumexp(logl1, logl2);
}

void StraightLine::print(std::ostream& out) const
{
	out<<m<<' '<<b;
}

string StraightLine::description() const
{
	return string("m, b");
}

