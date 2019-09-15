#include <iostream>
#include "DNest4/code/DNest4.h"
#include "MyModel.h"

int main(int argc, char** argv)
{
    // Run with first scalar
    MyModel::set_mode(Mode::first);
    DNest4::start<MyModel>(argc, argv);
    system("/home/brewer/local/anaconda3/bin/python3 showresults.py");
    system("mv sample.txt sample_first.txt");
    system("mv sample_info.txt sample_info_first.txt");
    system("mv log_prior_weights.txt log_prior_weights_first.txt");

    // Now run with second scalar
    MyModel::set_mode(Mode::second);
    DNest4::start<MyModel>(argc, argv);
    system("/home/brewer/local/anaconda3/bin/python3 showresults.py");
    system("mv sample.txt sample_second.txt");
    system("mv sample_info.txt sample_info_second.txt");
    system("mv log_prior_weights.txt log_prior_weights_second.txt");

    // Load the data from the first two runs and then target the sum
    MyModel::load_data();
    MyModel::set_mode(Mode::sum);
    DNest4::start<MyModel>(argc, argv);
    system("/home/brewer/local/anaconda3/bin/python3 showresults.py");
    system("mv sample.txt sample_final.txt");
    system("mv sample_info.txt sample_info_final.txt");
    system("mv log_prior_weights.txt log_prior_weights_final.txt");

    return 0;
}

