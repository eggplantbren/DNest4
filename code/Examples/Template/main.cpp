#include <iostream>
#include "DNest4/code/Start.h"
#include "MyModel.h"

using namespace DNest4;

int main(int argc, char** argv)
{
	DNest4::start<MyModel>(argc, argv);
	return 0;
}

