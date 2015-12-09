#include "RNG.h"
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
	return pow(10., 1.5 - 6.*this->rand())*this->randn();
}

int RNG::rand_int(int N)
{
	return static_cast<int>(floor(N*this->rand()));
}

} // namespace DNest4

