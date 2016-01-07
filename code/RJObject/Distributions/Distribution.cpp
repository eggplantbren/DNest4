#include "Distribution.h"

using namespace std;
using namespace DNest4;

Distribution::Distribution()
{

}

Distribution::~Distribution()
{

}

double Distribution::perturb1(RNG& rng, const vector< vector<double> >& components,
				vector< vector<double> >& u_components)
{
	double logH = 0.;

	// 1/(old density)
	for(size_t i=0; i<components.size(); i++)
		logH -= log_pdf(components[i]);

	logH += perturb_parameters(rng);

	// (new density)
	for(size_t i=0; i<components.size(); i++)
	{
		logH += log_pdf(components[i]);
		u_components[i] = components[i];
		to_uniform(u_components[i]);
	}

	return logH;
}

double Distribution::perturb2(RNG& rng, vector< vector<double> >& components,
				const vector< vector<double> >& u_components)
{
	double logH = 0.;

	logH += perturb_parameters(rng);

	// Find new positions
	for(size_t i=0; i<components.size(); i++)
	{
		components[i] = u_components[i];
		from_uniform(components[i]);
	}

	return logH;
}

