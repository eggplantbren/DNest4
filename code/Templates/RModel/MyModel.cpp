#include "MyModel.h"
#include "DNest4/code/DNest4.h"
#include <iomanip>
#include <Rinternals.h>
#include <Rembedded.h>
#include <cstring>

/* we have to disable stack check since R API will be called from
   different threads with different stacks */
extern "C" uintptr_t R_CStackLimit;

MyModel::MyModel()
{
    static int count = 0;
    if (count == 0) {
        const char *argv[4] = { "R", "--no-save", "--no-restore", 0 };
        Rmux.lock();
        Rf_initEmbeddedR(3, (char**) argv);
        R_CStackLimit = -1;
         std::cout << "# WARNING: Do not use more than one thread." << std::endl;
        /* call source("MyModel.R") in R */
        Rf_eval(PROTECT(Rf_lang2(Rf_install("source"),        PROTECT(Rf_mkString("MyModel.R")))), R_GlobalEnv);
        UNPROTECT(2);
        Rmux.unlock();
        ++count;
     }

     /* Make params the correct size by evaluating the num_params symbol in R */
   Rmux.lock();
    int np = Rf_asInteger(Rf_eval(Rf_install("num_params"), R_GlobalEnv));
    params = std::vector<double>(np);
    Rmux.unlock();
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
    Rmux.lock();
    /* create double() params vector */
    SEXP us = PROTECT(Rf_allocVector(REALSXP, params.size()));
    memcpy(REAL(us), &params[0], params.size() * sizeof(double));
    /* call log_likelihood() on that vector */
    SEXP res = Rf_eval(PROTECT(Rf_lang2(Rf_install("both"), us)), R_GlobalEnv);
    /* expect single double output */
    double r = Rf_asReal(res);
    UNPROTECT(2);
    Rmux.unlock();
    return r;
}

void MyModel::print(std::ostream& out) const
{
    out << std::setprecision(12);

    Rmux.lock();
    /* create double() params vector */
    SEXP us = PROTECT(Rf_allocVector(REALSXP, params.size()));
    memcpy(REAL(us), &params[0], params.size() * sizeof(double));
    /* call from_uniform() on that vector */
    SEXP res = Rf_eval(PROTECT(Rf_lang2(Rf_install("from_uniform"), us)), R_GlobalEnv);
    /* result is a doubles vector */
    int n = LENGTH(res);
    double *params2 = REAL(res);
    for(int i=0; i<n; ++i)
         out << params2[i] << ' ';
    UNPROTECT(2);
    Rmux.unlock();
}

std::string MyModel::description() const
{
    return std::string("");
}

