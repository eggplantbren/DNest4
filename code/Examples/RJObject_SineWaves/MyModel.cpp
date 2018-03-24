#include "MyModel.h"
#include "DNest4/code/DNest4.h"
#include "Data.h"
#include <cmath>
#include <sstream>

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

void MyModel::calculate_mu(bool update)
{
	// Get the times from the data
	const vector<double>& t = Data::get_instance().get_t();

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

        // Pre-rejection
        if(rng.rand() >= exp(logH))
            return -1E300;
        else
            logH = 0.0;

		calculate_mu(objects.get_removed().size() == 0);
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

void MyModel::read(std::istream& in)
{
    double junk;
	for(size_t i=0; i<mu.size(); i++)
		in>>junk;
	in>>sigma;
    log_sigma = log(sigma);

	objects.read(in);
    calculate_mu();
}

string MyModel::description() const
{
    stringstream s;

    // Anything printed by MyModel::print (except the last line)
    for(size_t i=0; i<mu.size(); i++)
        s<<"mu ["<<i<<"], ";
    s<<"sigma, ";

    // The rest is all what happens when you call .print on an RJObject
    s<<"dim_components, max_num_components, ";

    // Then the hyperparameters (i.e. whatever MyConditionalPrior::print prints)
    s<<"location_log_period, scale_log_period, ";
    s<<"location_log_amplitude, scale_log_amplitude, ";

    // Then the actual number of components
    s<<"num_components, ";

    // Then it's all the components, padded with zeros
    // max_num_components is 100 in this model, so that's how far the
    // zero padding goes.
    for(int i=0; i<100; ++i)
        s<<"log_period["<<i<<"], ";
    for(int i=0; i<100; ++i)
        s<<"log_amplitude["<<i<<"], ";
    for(int i=0; i<100; ++i)
        s<<"phase["<<i<<"], ";
    return s.str();
}

