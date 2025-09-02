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

    for(double x: xs)
        logL += -0.5*pow((x - 0.5)/0.01, 2);

    return logL;
}

