#include <iostream>
#include "DNest4/code/DNest4.h"
#include "MyModel.h"
#include <julia.h>
#include <cstdlib>

using namespace std;
using namespace DNest4;

int main(int argc, char** argv)
{
    jl_init();

    // Load and run the Julia script
    jl_eval_string("include(\"mymodel.jl\")");

    // Access the global variable `x` in the Julia script
    jl_value_t* julia_var = jl_eval_string("num_params");

    // Check if the returned value is an integer
    int64_t num_params = jl_unbox_int64(julia_var);
    MyModel::set_size(num_params);


	Sampler<MyModel> sampler = setup<MyModel>(argc, argv);
	sampler.run();

	jl_atexit_hook(0);

	return 0;
}

