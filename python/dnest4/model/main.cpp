#include <iostream>
#include "Data.h"
#include "DNest4/code/DNest4.h"
#include "StraightLine.h"

using namespace std;
using namespace DNest4;

int main(int argc, char** argv)
{
	Data::get_instance().load("road.txt");
	start<StraightLine>(argc, argv);
	return 0;
}

