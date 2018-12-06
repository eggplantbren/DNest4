#include "Rosenbrock.h"
#include "DNest4/code/DNest4.h"

using namespace std;
using namespace DNest4;

Rosenbrock::Rosenbrock()
:params(50)
{

}

void Rosenbrock::from_prior(RNG& rng)
{
	for(size_t i=0; i<params.size(); i++)
		params[i] = -10.0 + 20.0*rng.rand();
}

double Rosenbrock::perturb(RNG& rng)
{
    int reps = 1;
    if(rng.rand() < 0.5)
        reps += static_cast<int>(pow(params.size(), rng.rand()));

    int which;
    for(int i=0; i<reps; ++i)
    {
        which = rng.rand_int(params.size());
        params[which] += 20.0*rng.randh2();
        wrap(params[which], -10.0, 10.0);
    }

	return 0.0;
}

double Rosenbrock::log_likelihood() const
{
    double logL = 0.0;

    for(size_t i=0; i<(params.size()-1); ++i)
    {
        logL -= 100*pow(params[i+1] - params[i]*params[i], 2);
        logL -= pow(1.0 - params[i], 2);
    }

    logL *= 2;

    return logL;
}

void Rosenbrock::print(std::ostream& out) const
{
	for(const double& x: params)
		out<<x<<' ';
}

string Rosenbrock::description() const
{
	return string("Each column is one of the 20 parameters.");
}

