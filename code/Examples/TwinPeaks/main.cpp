#include <iostream>
#include "DNest4/code/DNest4.h"
#include "MyModel.h"

using namespace DNest4;

int main(int argc, char** argv)
{
    CommandLineOptions cl_options(argc, argv);

    // Run with first scalar
    MyModel::set_mode(Mode::first);
	Sampler<MyModel> sampler = setup<MyModel>(cl_options);
	sampler.run();
    system("/home/brewer/local/anaconda3/bin/python3 showresults.py");
    system("cp sample.txt sample_first.txt");
    system("cp sample_info.txt sample_info_first.txt");
    system("cp log_prior_weights.txt log_prior_weights_first.txt");

    // Now run with second scalar
    MyModel::set_mode(Mode::second);
	sampler = setup<MyModel>(cl_options);
	sampler.run();
    system("/home/brewer/local/anaconda3/bin/python3 showresults.py");
    system("cp sample.txt sample_second.txt");
    system("cp sample_info.txt sample_info_second.txt");
    system("cp log_prior_weights.txt log_prior_weights_second.txt");

    // Load the data from the first two runs and then target the sum
    MyModel::load_data();
    MyModel::set_mode(Mode::sum);
	sampler = setup<MyModel>(cl_options);
	sampler.run();
    system("/home/brewer/local/anaconda3/bin/python3 showresults.py");
    system("cp sample.txt sample_final.txt");
    system("cp sample_info.txt sample_info_final.txt");
    system("cp log_prior_weights.txt log_prior_weights_final.txt");

    return 0;
}

