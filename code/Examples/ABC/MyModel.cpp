#include "MyModel.h"
#include "DNest4/code/DNest4.h"
#include "Data.h"
#include <cmath>

using namespace std;
using namespace DNest4;

MyModel::MyModel()
:n(Data::get_instance().get_x().size())
{

}

void MyModel::from_prior(RNG& rng)
{
	mu = -10. + 20.*rng.rand();
	log_sigma = -10. + 20.*rng.rand();
	for(size_t i=0; i<n.size(); i++)
		n[i] = rng.randn();
}

double MyModel::perturb(RNG& rng)
{
	int which = rng.rand_int(3);

	if(which == 0)
	{
		mu += 20.*rng.randh();
		wrap(mu, -10., 10.);
	}
	if(which == 1)
	{
		log_sigma += 20.*rng.randh();
		wrap(log_sigma, -10., 10.);
	}
	if(which == 2)
	{
		int i = rng.rand_int(n.size());
		n[i] = rng.randn();
	}

	return 0.;
}

double MyModel::log_likelihood() const
{
	double x_min = Data::get_instance().get_x_min();
	double x_max = Data::get_instance().get_x_max();

	double sigma = exp(log_sigma);

	// Assemble fake dataset
	vector<double> x_fake = n;
	for(size_t i=0; i<x_fake.size(); i++)
		x_fake[i] = mu + sigma*x_fake[i];

	// Goodness
	double logL = 0.;
	logL -= pow(*min_element(x_fake.begin(), x_fake.end()) - x_min, 2);
	logL -= pow(*max_element(x_fake.begin(), x_fake.end()) - x_max, 2);

	return logL;
}

void MyModel::print(std::ostream& out) const
{
	out<<mu<<' '<<log_sigma<<' ';
	for(size_t i=0; i<n.size(); i++)
		out<<n[i]<<' ';
}

string MyModel::description() const
{
	return string("");
}

