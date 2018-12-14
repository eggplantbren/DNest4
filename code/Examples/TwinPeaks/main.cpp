#include <iostream>
#include "DNest4/code/DNest4.h"
#include "MyModel.h"

int main(int argc, char** argv)
{

    // Run with first scalar
    MyModel::set_mode(Mode::first);
    DNest4::start<MyModel>(argc, argv);
    system("mv sample_info.txt first.txt");

    // Now run with second scalar
    MyModel::set_mode(Mode::second);
    DNest4::start<MyModel>(argc, argv);
    system("mv sample_info.txt second.txt");

    // Load the data from the first two runs and then target the sum
    MyModel::load_data();
    MyModel::set_mode(Mode::sum);
    DNest4::start<MyModel>(argc, argv);

    return 0;
}

