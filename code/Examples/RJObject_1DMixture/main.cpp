#include <iostream>
#include "DNest4/code/DNest4.h"
#include "MyModel.h"
#include "Data.h"

using namespace DNest4;

int main(int argc, char** argv)
{
    Data::get_instance().load("galaxies.txt");
    DNest4::start<MyModel>(argc, argv);
    return 0;
}

