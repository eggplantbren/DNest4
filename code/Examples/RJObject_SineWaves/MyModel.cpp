#include "MyModel.h"
#include "Utils.h"
#include "Data.h"
#include <cmath>

using namespace std;
using namespace DNest4;

MyModel::MyModel()
:objects(3, 10, false, MyConditionalPrior(-10., 10., 1E-3, 1E3))
,mu(Data::get_instance().get_t().size())
{

}

void MyModel::from_prior(RNG& rng)
{
	objects.from_prior(rng);
	objects.consolidate_diff();
	sigma = exp(log(1E-3) + log(1E6)*rng.rand());
	calculate_mu();
}

void MyModel::calculate_mu()
{
	// Get the times from the data
	const vector<double>& t = Data::get_instance().get_t();

	// Update or from scratch?
	bool update = false;//(objects.get_added().size() < objects.get_components().size());

	// Get the components
	const vector< vector<double> >& components = (update)?(objects.get_added()):
				(objects.get_components());

	// Zero the signal
	if(!update)
		mu.assign(mu.size(), 0.);

	double T, A, phi;
	for(size_t j=0; j<components.size(); j++)
	{
		T = exp(components[j][0]);
		A = components[j][1];
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
		objects.consolidate_diff();
		calculate_mu();
	}
	else
	{
		sigma = log(sigma);
		sigma += log(1E6)*rng.randh();
		sigma = mod(sigma - log(1E-3), log(1E6)) + log(1E-3);
		sigma = exp(sigma);
	}

	return logH;
}

double MyModel::log_likelihood() const
{
	// Get the data
	const vector<double>& y = Data::get_instance().get_y();

	double logL = 0.;
	double var = sigma*sigma;
	for(size_t i=0; i<y.size(); i++)
		logL += -0.5*log(2.*M_PI*var) - 0.5*pow(y[i] - mu[i], 2)/var;

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

