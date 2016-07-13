#include <iostream>
#include "DNest4/code/DNest4.h"
#include "Rosenbrock.h"

using namespace DNest4;

int main(int argc, char** argv)
{
	start<Rosenbrock>(argc, argv);
	return 0;
}

