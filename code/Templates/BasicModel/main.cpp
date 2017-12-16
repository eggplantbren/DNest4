#include <iostream>
#include "DNest4/code/DNest4.h"
#include "MyModel.h"

int main(int argc, char** argv)
{
    DNest4::start<MyModel>(argc, argv);
    return 0;
}

