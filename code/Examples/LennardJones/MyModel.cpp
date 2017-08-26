#include "MyModel.h"
#include "DNest4/code/DNest4.h"

using namespace std;
using namespace DNest4;

MyModel::MyModel()
{

}

void MyModel::from_prior(RNG& rng)
{

}

double MyModel::perturb(RNG& rng)
{
	double logH = 0.;

	return logH;
}

double MyModel::log_likelihood() const
{
	double logL = 0.;
	return logL;
}

void MyModel::print(std::ostream& out) const
{

}

string MyModel::description() const
{
	return string("");
}

