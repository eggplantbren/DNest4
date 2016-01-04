#include <iostream>
#include "Data.h"
#include "Start.h"
#include "StraightLine.h"

using namespace std;
using namespace DNest4;

int main(int argc, char** argv)
{
	Data::get_instance().load("road.txt");

//	start<SpikeSlab>(argc, argv);
	return 0;
}

