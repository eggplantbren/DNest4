#include "MyModel.h"
#include "DNest4/code/DNest4.h"
#include "Data.h"
#include <cmath>

using namespace std;
using namespace DNest4;

const Cauchy MyModel::cauchy(0.0, 5.0);

MyModel::MyModel()
:objects(3, 100, false, MyConditionalPrior(), PriorType::log_uniform)
,mu(Data::get_instance().get_t().size())
{

}

void MyModel::from_prior(RNG& rng)
{
	objects.from_prior(rng);
	log_sigma = cauchy.generate(rng);
    sigma = exp(log_sigma);
	calculate_mu();
}

void MyModel::calculate_mu()
{
	// Get the times from the data
	const vector<double>& t = Data::get_instance().get_t();

	// Update or from scratch?
	bool update = (objects.get_removed().size() == 0);

	// Get the components
	const vector< vector<double> >& components = (update)?(objects.get_added()):
				                                          (objects.get_components());

	// Zero the signal
	if(!update)
		mu.assign(mu.size(), 0.0);

	double T, A, phi;
	for(size_t j=0; j<components.size(); j++)
	{
		T = exp(components[j][0]);
		A = exp(components[j][1]);
		phi = components[j][2];
		for(size_t i=0; i<t.size(); i++)
			mu[i] += A*sin(2.*M_PI*t[i]/T + phi);
	}
}

double MyModel::perturb(RNG& rng)
{
	double logH = 0.;

	if(rng.rand() <= 0.75)
	{
		logH += objects.perturb(rng);
		calculate_mu();
	}
	else
	{
		logH += cauchy.perturb(log_sigma, rng);
        sigma = exp(log_sigma);
	}

	return logH;
}

double MyModel::log_likelihood() const
{
	// Get the data
	const vector<double>& y = Data::get_instance().get_y();
    const vector<double>& sig = Data::get_instance().get_sig();

	double logL = 0.;
	double var;
	for(size_t i=0; i<y.size(); i++)
    {
        var = sig[i]*sig[i] + sigma*sigma;
		logL += -0.5*log(2.*M_PI*var) - 0.5*pow(y[i] - mu[i], 2)/var;
    }

	return logL;
}

void MyModel::print(std::ostream& out) const
{
	for(size_t i=0; i<mu.size(); i++)
		out<<mu[i]<<' ';
	out<<sigma<<' ';
	objects.print(out); out<<' ';
}

string MyModel::description() const
{
	return string("objects, sigma");
}

