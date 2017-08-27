#include "MyModel.h"
#include "DNest4/code/DNest4.h"

using namespace std;
using namespace DNest4;

MyModel::MyModel()
:x(num_atoms)
,y(num_atoms)
,z(num_atoms)
{
    // Allocate space for the potential energy values
    PE.resize(num_atoms);
    for(size_t i=0; i<num_atoms; ++i)
        PE[i].resize(num_atoms);
}

void MyModel::from_prior(RNG& rng)
{
    // Uniform prior for positions
    for(size_t i=0; i<num_atoms; ++i)
    {
        x[i] = L*rng.rand();
        y[i] = L*rng.rand();
        z[i] = L*rng.rand();
    }

    compute_PE();
}

void MyModel::compute_PE()
{
    for(size_t i=0; i<num_atoms; ++i)
        for(size_t j=0; j<i; ++j)
            compute_PE(i, j);
}

void MyModel::compute_PE(size_t i, size_t j)
{
    double rsq = pow(x[i] - x[j], 2) +
                 pow(y[i] - y[j], 2) +
                 pow(z[i] - z[j], 2);

    PE[i][j] = 4*(pow(rsq, -6) - pow(rsq, -3));
    PE[j][i] = PE[i][j];
}

double MyModel::perturb(RNG& rng)
{
    double logH = 0.0;

    // Choose a particle
    int which = rng.rand_int(num_atoms);
    int coord = rng.rand_int(3);

    if(coord == 0)
    {
        x[which] += L*rng.randh();
        wrap(x[which], 0.0, L);
    }
    else if(coord == 1)
    {
        y[which] += L*rng.randh();
        wrap(y[which], 0.0, L);
    }
    else
    {
        z[which] += L*rng.randh();
        wrap(z[which], 0.0, L);
    }

    for(size_t i=0; i<num_atoms; ++i)
        if((int)i != which)
            compute_PE(i, which);

    return logH;
}

double MyModel::log_likelihood() const
{
    double logL = 0.0;

    for(size_t i=0; i<num_atoms; ++i)
        for(size_t j=0; j<i; ++j)
            logL += -PE[i][j];

    return logL;
}

void MyModel::print(std::ostream& out) const
{
    for(size_t i=0; i<num_atoms; ++i)
        out << x[i] << ' ';
    for(size_t i=0; i<num_atoms; ++i)
        out << y[i] << ' ';
    for(size_t i=0; i<num_atoms; ++i)
        out << z[i] << ' ';

}

string MyModel::description() const
{
    return string("");
}

