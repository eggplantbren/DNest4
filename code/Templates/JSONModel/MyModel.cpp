#include "MyModel.h"
#include "DNest4/code/DNest4.h"
#include <iomanip>

thread_local ModelServer server("python3 mymodel.py");


MyModel::MyModel()
:us(server.get_num_params())
,params(server.get_num_params())
{

}

void MyModel::from_prior(DNest4::RNG& rng)
{
    for(double& u: us)
        u = rng.rand();

    // Also compute the transformed parameters
    const_cast<MyModel*>(this)->params = server.prior_transform(us);

    logl = server.log_likelihood(params);
}

double MyModel::perturb(DNest4::RNG& rng)
{
    double logH = perturb_us(us, rng);

    // Also compute the transformed parameters
    const_cast<MyModel*>(this)->params = server.prior_transform(us);
    logl = server.log_likelihood(params);

    return logH;
}

double MyModel::log_likelihood() const
{
    return logl;
}

void MyModel::print(std::ostream& out) const
{
out.unsetf(std::ios::floatfield);  // restore defaultfloat
out << std::setprecision(12);
    for(const double& param: params)
        out << param << ' ';
}

std::string MyModel::description() const
{
    return std::string("");
}

