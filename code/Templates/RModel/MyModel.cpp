#include "MyModel.h"
#include "DNest4/code/DNest4.h"
#include <iomanip>
#include <RInside.h>

RInside MyModel::R;

MyModel::MyModel()
{
    static int count = 0;
    if(count == 0)
    {
        std::cout << "# WARNING: Do not use more than one thread." << std::endl;
        R.parseEvalQ("source(\"MyModel.R\")");
    }
    ++count;

    // Make params the correct size
    Rcpp::NumericVector num_params = R.parseEval("num_params");
    params = std::vector<double>(num_params[0]);
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
        params[k] += rng.randh2();
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
    Rcpp::NumericVector us(params.size());
    for(size_t i=0; i<params.size(); ++i)
        us[i] = params[i];
    R["us"] = us;

    Rcpp::NumericVector logL = R.parseEval("log_likelihood(us)");
    return logL[0];
}

void MyModel::print(std::ostream& out) const
{
    out << std::setprecision(12);

    Rcpp::NumericVector us(params.size());
    for(size_t i=0; i<params.size(); ++i)
        us[i] = params[i];
    R["us"] = us;
    Rcpp::NumericVector params2 = R.parseEval("from_uniform(us)");

    for(int i=0; i<params2.size(); ++i)
        out << params2[i] << ' ';
}

std::string MyModel::description() const
{
    return std::string("");
}

