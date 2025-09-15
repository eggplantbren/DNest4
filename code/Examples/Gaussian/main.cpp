#include <iostream>
#include "DNest4/code/DNest4.h"
#include "GaussianExample.h"

using namespace DNest4;

int main(int argc, char** argv)
{
    RNG::randh_is_randh2 = true;
	start<GaussianExample>(argc, argv);


	std::cout << "Analytical solution: " << GaussianExample().analytic_log_Z() << std::endl;

	return 0;
}

