#include <iostream>
#include "DNest4/code/DNest4.h"
#include "MyModel.h"
#include <julia/julia.h>
#include <cstdlib>

using namespace std;
using namespace DNest4;

int main(int argc, char** argv)
{
	jl_init_with_image(getenv("JULIA_LIB_PATH"), "sys.so");
	jl_eval_string("include(\"julia_model.jl\")");

	Sampler<MyModel> sampler = setup<MyModel>(argc, argv);
	sampler.run();

	jl_atexit_hook(0);

	return 0;
}

