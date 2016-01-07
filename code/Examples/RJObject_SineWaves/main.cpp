#include <iostream>
#include "Data.h"
#include "Start.h"
#include "MyModel.h"

using namespace std;
using namespace DNest4;

int main(int argc, char** argv)
{
	Data::get_instance().load("fake_data.txt");
	start<MyModel>(argc, argv);
	return 0;
}

