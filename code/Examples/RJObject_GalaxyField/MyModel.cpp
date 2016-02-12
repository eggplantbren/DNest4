#include "MyModel.h"
#include "DNest4/code/DNest4.h"
#include "Data.h"
#include <cmath>

using namespace std;
using namespace DNest4;

MyModel::MyModel()
:objects(8, 100, false, MyConditionalPrior(
	Data::get_instance().get_x_min(), Data::get_instance().get_x_max(),
	Data::get_instance().get_y_min(), Data::get_instance().get_y_max(),
			1E-3, 1E3))
,image(Data::get_instance().get_ni(),
	vector<long double>(Data::get_instance().get_nj()))
,staleness(0)
{

}

void MyModel::from_prior(RNG& rng)
{
	objects.from_prior(rng);
	calculate_image();
	sigma = exp(log(1.) + log(1E6)*rng.rand());
}

void MyModel::calculate_image()
{
	// Get coordinate stuff from data
	const vector< vector<double> >& x = Data::get_instance().get_x_rays();
	const vector< vector<double> >& y = Data::get_instance().get_y_rays();

	// Components
	const vector< vector<double> >& components = objects.get_components();

	// Diff
	const vector< vector<double> >& added = objects.get_added();
	const vector< vector<double> >& removed = objects.get_removed();
	bool update = (added.size() + removed.size() < components.size()) &&
				(staleness < 100);
	int num = (update)?(added.size() + removed.size()):(components.size());

	if(update)
		staleness++;
	else
	{
		staleness = 0;
		// Zero the image
		image.assign(Data::get_instance().get_ni(),
			vector<long double>(Data::get_instance().get_nj(), 0.));
	}

	double xc, yc, M, w, q, theta, cos_theta, sin_theta, wsq;
	double rinner, rinnersq, frac;
	double xx, yy, rsq;

	const vector<double>* component;
	double coeff = 1.; // switches to -1 for removing components

	for(int k=0; k<num; k++)
	{
		if(update)
		{
			if(k < (int)added.size())
			{
				component = &(added[k]);
				coeff = 1.;
			}
			else
			{
				component = &(removed[k - (int)added.size()]);
				coeff = -1.;
			}
		}
		else
			component = &(components[k]);

		xc = (*component)[0]; yc = (*component)[1];
		M = (*component)[2]; w = (*component)[3];
		q = (*component)[4]; theta = (*component)[5];
		cos_theta = cos(theta); sin_theta = sin(theta);
		wsq = w*w;
		rinner = (*component)[6]*w;
		rinnersq = rinner*rinner;
		frac = (*component)[7];

		for(size_t i=0; i<image.size(); i++)
		{
			for(size_t j=0; j<image[i].size(); j++)
			{
				xx =  (x[i][j] - xc)*cos_theta + (y[i][j] - yc)*sin_theta;
				yy = -(x[i][j] - xc)*sin_theta + (y[i][j] - yc)*cos_theta;
				rsq = q*xx*xx + yy*yy/q;
				// Outer gaussian
				if(rsq < 25.*wsq)
					image[i][j] += coeff*(1. - frac)*M/(2.*M_PI*wsq)*exp(-0.5*rsq/wsq);
				// Inner gaussian
				if(rsq < 25.*rinnersq)
					image[i][j] += coeff*frac*M/(2.*M_PI*rinnersq)*exp(-0.5*rsq/rinnersq);
			}
		}
	}
}

double MyModel::perturb(RNG& rng)
{
	double logH = 0.;

	if(rng.rand() <= 0.75)
	{
		logH += objects.perturb(rng);
		calculate_image();
	}
	else
	{
		sigma = log(sigma);
		sigma += log(1E6)*rng.randh();
		sigma = mod(sigma - log(1.), log(1E6)) + log(1.);
		sigma = exp(sigma);
	}

	return logH;
}

double MyModel::log_likelihood() const
{
	const vector< vector<double> >& data = Data::get_instance().get_image();
	const vector< vector<double> >& sig = Data::get_instance().get_sigma();

	double logL = 0.;
	double var;
	for(size_t i=0; i<data.size(); i++)
	{
		for(size_t j=0; j<data[i].size(); j++)
		{
			var = sigma*sigma + sig[i][j]*sig[i][j];
			logL += -0.5*log(2.*M_PI*var)
				-0.5*pow(data[i][j] - image[i][j], 2)/var;
		}
	}

	return logL;
}

void MyModel::print(std::ostream& out) const
{
	out<<setprecision(5);
	for(size_t i=0; i<image.size(); i++)
		for(size_t j=0; j<image[i].size(); j++)
			out<<image[i][j]<<' ';
	out<<setprecision(10);
	objects.print(out); out<<' ';
	out<<sigma<<' '<<staleness<<' ';
}

string MyModel::description() const
{
	return string("objects");
}

