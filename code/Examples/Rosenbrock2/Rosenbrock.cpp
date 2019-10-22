#include "Rosenbrock.h"
#include "DNest4/code/DNest4.h"
#include <iomanip>

using namespace std;
using namespace DNest4;

Rosenbrock::Rosenbrock()
:xs(n2, std::vector<double>(n1))
{

}

void Rosenbrock::from_prior(RNG& rng)
{
    // First column is all the same value
    xs[0][0] = -1E6 + 2E6*rng.rand();
    for(int i=1; i<n2; ++i)
        xs[i][0] = xs[0][0];

    // Free values for the rest
    for(int i=0; i<n2; ++i)
        for(int j=1; j<n1; ++j)
            xs[i][j] = -1E6 + 2E6*rng.rand();
}

double Rosenbrock::perturb(RNG& rng)
{
    int reps = 1;
//    if(rng.rand() < 0.5)
//        reps += static_cast<int>(pow(n1*n2, rng.rand()));
   
    int which_i, which_j;
    for(int rep=0; rep<reps; ++rep)
    {
        which_i = rng.rand_int(n2);
        which_j = rng.rand_int(n1);

        double& xx = xs[which_i][which_j];
        xx += 2E6*rng.randh();
        wrap(xx, -1E6, 1E6);

        // First column is all the same value
        if(which_j == 0)
        {
            for(int i=0; i<n2; ++i)
                if(i != which_i)
                    xs[i][0] = xx;
        }
    }

	return 0.0;
}

double Rosenbrock::log_likelihood() const
{
    double logL = 0.0;

    // Weird dot part
    logL -= a*pow(xs[0][0] - mu, 2);

    // Loop over all columns starting from the second one
    for(int j=1; j<n1; ++j)
    {
        for(int i=0; i<n2; ++i)
            logL -= b*pow(xs[i][j] - pow(xs[i][j-1], 2), 2);
    }

    return logL;
}

void Rosenbrock::print(std::ostream& out) const
{
    out << std::setprecision(12);

    // First column value
    out << xs[0][0] << ' ';

    // The rest
    for(int i=0; i<n2; ++i)
        for(int j=1; j<n1; ++j)
		    out << xs[i][j] <<' ';
}

string Rosenbrock::description() const
{
	return string("");
}

