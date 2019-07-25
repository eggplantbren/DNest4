#include <iostream>
#include "DNest4/code/DNest4.h"
#include "MyModel.h"

int main(int argc, char** argv)
{
    // Use randh2
    DNest4::RNG::randh_is_randh2 = true;

    // Run sampler
	DNest4::start<MyModel>(argc, argv);

	return 0;
}

