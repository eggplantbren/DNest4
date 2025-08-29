#include "MyModel.h"
#include "DNest4/code/DNest4.h"

MyModel::MyModel()
{

}

void MyModel::prior_transform()
{
    xs = us;
}

double MyModel::log_likelihood() const
{
    double logL = 0.0;
    return logL;
}

void MyModel::print(std::ostream& out) const
{

}

std::string MyModel::description() const
{
    return std::string("");
}

