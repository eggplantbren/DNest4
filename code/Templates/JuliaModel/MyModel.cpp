#include "MyModel.h"
#include "DNest4/code/DNest4.h"
#include <cmath>
#include <julia.h>

using namespace std;
using namespace DNest4;

int MyModel::size = 1;

MyModel::MyModel()
:us(size)
{

}

void MyModel::set_size(int _size)
{
    size = _size;
}

void MyModel::from_prior(RNG& rng)
{
	for(size_t i=0; i<us.size(); i++)
		us[i] = rng.rand();
}

double MyModel::perturb(RNG& rng)
{
	int which, reps;
	if(rng.rand() <= 0.5)
		reps = 1;
	else
		reps = static_cast<int>(pow(us.size(), 2*rng.rand()));

	for(int i=0; i<reps; i++)
	{
		which = rng.rand_int(us.size());
		us[which] += rng.randh();
		wrap(us[which], 0.0, 1.0);
	}

	return 0.;
}

double MyModel::log_likelihood() const
{
    // Convert std::vector<double> to a Julia array (Vector{Float64})
    jl_value_t* array_type = jl_apply_array_type((jl_value_t*)jl_float64_type, 1);  // Create a Julia array type for Float64
    jl_array_t* julia_array = jl_alloc_array_1d(array_type, us.size());  // Allocate a Julia array

    // Copy data from std::vector<double> to the Julia array
    double* julia_array_data = jl_array_data(julia_array, double);
    std::copy(us.begin(), us.end(), julia_array_data);

    // Get the function `sum_vector` from the Julia global scope
    jl_function_t *both_func = jl_get_function(jl_main_module, "both");

    // Call the Julia function with the Julia array as an argument
    jl_value_t* args = (jl_value_t*)julia_array;
    jl_value_t* result = jl_call1(both_func, args);

    double result_double = jl_unbox_float64(result);

    return result_double;
}

void MyModel::print(std::ostream& out) const
{
    // Convert std::vector<double> to a Julia array (Vector{Float64})
    jl_value_t* array_type = jl_apply_array_type((jl_value_t*)jl_float64_type, 1);  // Create a Julia array type for Float64
    jl_array_t* julia_array = jl_alloc_array_1d(array_type, us.size());  // Allocate a Julia array

    // Copy data from std::vector<double> to the Julia array
    double* julia_array_data = jl_array_data(julia_array, double);
    std::copy(us.begin(), us.end(), julia_array_data);

   // Get the function `prior_transform` from the Julia global scope
    jl_function_t *prior_transform_func = jl_get_function(jl_main_module, "prior_transform");

    // Call the Julia function with the Julia array as an argument
    jl_value_t* args2 = (jl_value_t*)julia_array;
    jl_value_t* result2 = jl_call1(prior_transform_func, args2);

    jl_array_t* result_array = (jl_array_t*)result2;
    double* result_data = jl_array_data(result_array, double);
    size_t len = jl_array_len(result_array);

    // Create a C++ vector and copy the data from Julia array
    std::vector<double> cpp_result(result_data, result_data + len);

    // Print the result vector
    out << std::setprecision(12);
    for (double val : cpp_result)
        out << val << ' ';
}

string MyModel::description() const
{
	return string("");
}

