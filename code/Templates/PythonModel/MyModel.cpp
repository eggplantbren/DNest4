#include "MyModel.h"
#include "DNest4/code/DNest4.h"
#include <iomanip>
#include <pybind11/numpy.h>

pybind11::scoped_interpreter MyModel::guard;
pybind11::module_ MyModel::my_module = pybind11::module_::import("mymodel");

int MyModel::size = 1;



MyModel::MyModel()
:params(size)
{

}

void MyModel::from_prior(DNest4::RNG& rng)
{
    for(double& x: params)
        x = rng.rand();
}

double MyModel::perturb(DNest4::RNG& rng)
{
    double logH = 0.0;

    int type = rng.rand_int(2);
    if(type == 0)
    {
        int k = rng.rand_int(params.size());
        params[k] += rng.randh();
        DNest4::wrap(params[k], 0.0, 1.0);
    }
    else
    {
        int reps = (int)pow(params.size(), rng.rand());
        for(int i=0; i<reps; ++i)
        {
            int k = rng.rand_int(params.size());
            params[k] += rng.randh();
            DNest4::wrap(params[k], 0.0, 1.0);
        }
    }

    return logH;
}

double MyModel::log_likelihood() const
{
    pybind11::array_t<double> numpy_array(params.size(),
                                          params.data());

    static pybind11::object both = my_module.attr("both");
    double result = both(numpy_array).cast<double>();

    return result;
}

void MyModel::set_size(int _size)
{
    size = _size;
}

void MyModel::print(std::ostream& out) const
{
    out << std::setprecision(12);

    pybind11::array_t<double> numpy_array(params.size(),
                                          params.data());
    static pybind11::object prior_transform = my_module.attr("prior_transform");
    pybind11::array_t<double> xs =
                prior_transform(numpy_array).cast<pybind11::array_t<double>>();

    for(int i=0; i<size; ++i)
        out << xs.data()[i] << ' ';
}

std::string MyModel::description() const
{
    return std::string("");
}

