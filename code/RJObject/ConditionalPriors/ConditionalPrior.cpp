#include "ConditionalPrior.h"

using namespace std;
using namespace DNest4;

ConditionalPrior::ConditionalPrior()
{

}

ConditionalPrior::~ConditionalPrior()
{

}

double ConditionalPrior::perturb1(RNG& rng, const vector< vector<double> >& components,
				vector< vector<double> >& u_components)
{
	double logH = 0.;

	// 1/(old density)
	for(size_t i=0; i<components.size(); i++)
		logH -= log_pdf(components[i]);

	logH += perturb_hyperparameters(rng);

	// (new density)
	for(size_t i=0; i<components.size(); i++)
	{
		logH += log_pdf(components[i]);
		u_components[i] = components[i];
		to_uniform(u_components[i]);

        for(size_t j=0; j<components[i].size(); ++j)
            if(u_components[i][j] <= 0.0 || u_components[i][j] >= 1.0)
                logH = -1E300;
	}

	return logH;
}

double ConditionalPrior::perturb2(RNG& rng, vector< vector<double> >& components,
				const vector< vector<double> >& u_components)
{
	double logH = 0.;

	logH += perturb_hyperparameters(rng);

	// Find new positions
	for(size_t i=0; i<components.size(); i++)
	{
		components[i] = u_components[i];
		from_uniform(components[i]);
	}

	return logH;
}

