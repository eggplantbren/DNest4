#include <iostream>
#include "DNest4/code/DNest4.h"
#include "MyModel.h"
#include "Data.h"

using namespace std;
using namespace DNest4;

int main(int argc, char** argv)
{
	Data::get_instance().load("data.txt");

	Sampler<MyModel> sampler = setup<MyModel>(argc, argv);
	sampler.run();

	return 0;
}

