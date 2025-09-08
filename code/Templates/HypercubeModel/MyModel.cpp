#include "MyModel.h"
#include "DNest4/code/DNest4.h"

MyModel::MyModel()
{

}

void MyModel::prior_transform()
{
    x = us[0];
    y = us[1];
}

double MyModel::log_likelihood() const
{
    double logL = 0.0;

    logL += -0.5*pow(x, 2) - 0.5*pow((y - x*x)/0.01, 2);

    return logL;
}

void MyModel::print(std::ostream& out) const
{
    out << x << ' ' << y;
}
