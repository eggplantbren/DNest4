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
    if(gaussians.get_components().size() == 0)
        return -std::numeric_limits<double>::max();

	return logH;
}

double MyModel::log_likelihood() const
{
	double logL = 0.0;

    // Extract parameters
    vector<double> mu, sigma, weight;
    const auto& components = gaussians.get_components();
    for(const auto& c: components)
    {
        mu.push_back(c[0]);
        sigma.push_back(exp(c[1]));
        weight.push_back(exp(c[2]));
    }

    // Normalise weights
    double tot = 0.0;
    for(double w: weight)
        tot += w;
    for(double& w: weight)
        w /= tot;

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
                            log(weight[j]) - 0.5*log(2*M_PI*sigma[j]*sigma[j])
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

