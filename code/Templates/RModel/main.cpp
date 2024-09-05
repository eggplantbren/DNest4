#include <iostream>
#include "DNest4/code/DNest4.h"
#include "MyModel.h"
#include <pybind11/embed.h>

int main(int argc, char** argv)
{

    // Get the number of parameters and give the value to our class
    pybind11::exec("from mymodel import *");
    pybind11::object num_params = pybind11::globals()["num_params"];
    MyModel::set_size(num_params.cast<int>());

    DNest4::start<MyModel>(argc, argv);


    return 0;
}

