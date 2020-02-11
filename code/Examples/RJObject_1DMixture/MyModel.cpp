#include "MyModel.h"
#include "DNest4/code/DNest4.h"
#include "Data.h"
#include <limits>

using namespace std;
using namespace DNest4;

MyModel::MyModel()
:gaussians(3, 100, false, MyConditionalPrior(), PriorType::log_uniform)
{

}

void MyModel::from_prior(RNG& rng)
{
    do
    {
        gaussians.from_prior(rng);
    }while(gaussians.get_components().size() == 0);
}

double MyModel::perturb(RNG& rng)
{
	double logH = 0.0;

    logH += gaussians.perturb(rng);

    // Don't allow zero components
    if(gaussians.get_components().size() == 0)
        return -1E300;

    // Pre-rejection
    if(rng.rand() >= exp(logH))
        return -1E300;
    else
        logH = 0.0;

    return logH;
}

double MyModel::log_likelihood() const
{
	double logL = 0.0;

    // Extract parameters
    vector<double> mu, sigma, log_weight, C;
    const auto& components = gaussians.get_components();

    if(components.size() == 0)
        return -1E300;

    for(const auto& c: components)
    {
        mu.push_back(c[0]);
        sigma.push_back(exp(c[1]) + 1.0);
        log_weight.push_back(c[2]);
        C.push_back(0.5*log(2*M_PI*pow(sigma.back(), 2)));
    }

    // Normalise weights
    double tot = logsumexp(log_weight);
    for(double& lw: log_weight)
        lw -= tot;

    // Get data
    const vector<double>& data = Data::get_instance().get_x();

    // Loop over data
    for(double x: data)
    {
        // Log-likelihood for a single data point
        double _logL = -std::numeric_limits<double>::max();

        // Loop over mixture components
        for(size_t j=0; j<mu.size(); ++j)
        {
            _logL = logsumexp(_logL,
                            log_weight[j] - C[j]
                            -0.5*pow((x - mu[j])/sigma[j], 2));
        }

        logL += _logL;
    }

	return logL;
}

void MyModel::print(std::ostream& out) const
{
    gaussians.print(out);
}

string MyModel::description() const
{
	return string("");
}

