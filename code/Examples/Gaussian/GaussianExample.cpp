#include "GaussianExample.h"
#include "DNest4/code/DNest4.h"

using namespace std;
using namespace DNest4;

GaussianExample::GaussianExample()
:params(5)
{

}

double GaussianExample::analytic_log_Z() {
    return this->params.size() * log(erf(0.5*this->width / sqrt(2))) - this->params.size() * log(this->width);
}

void GaussianExample::from_prior(RNG& rng)
{
    // Sample between -0.5 * width and +0.5 * width
	for(size_t i=0; i<params.size(); i++) {
        params[i] = -0.5 * width + width * rng.rand();
        std::cout << i << " " << params[i] << std::endl;
    }
}

double GaussianExample::perturb(RNG& rng)
{
    params[coordinate] += width*rng.randh2();
    wrap(params[coordinate], -0.5*width, 0.5*width);
    coordinate += 1;
    coordinate = coordinate % params.size();
	return 0.0;
}

double GaussianExample::log_likelihood() const
{
    double logL = 0.0;

    for(size_t i=0; i<params.size(); ++i)
    {
        logL += params[i] * params[i];
    }

    logL = -0.5 * logL;

    double constant = -0.5 * log(2* ( std::atan(1)*4  ) // pi
            );
    logL = logL + this->params.size() * constant;
    return logL;
}

void GaussianExample::print(std::ostream& out) const
{
	for(const double& x: params)
		out<<x<<' ';
}

string GaussianExample::description() const
{
	return string("Each column is one of the 5 parameters.");
}

