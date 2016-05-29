#include "RNG.h"
#include "Utils.h"
#include <cmath>

namespace DNest4
{

RNG::RNG()
:uniform(0., 1.)
,normal(0., 1.)
{

}

void RNG::set_seed(unsigned int seed)
{
	twister.seed(seed);
}

double RNG::rand()
{
	return uniform(twister);
}

double RNG::randn()
{
	return normal(twister);
}

double RNG::randh()
{
    // t-distribution with 2 degrees of freedom (less ridiculous than Cauchy)
    double T = this->randn()/
                sqrt((pow(this->randn(), 2) + pow(this->randn(), 2))/2);
	return pow(10.0, 1.5 - 3*std::abs(T))*this->randn();
}

int RNG::rand_int(int N)
{
	return static_cast<int>(floor(N*this->rand()));
}

} // namespace DNest4

