#include <iostream>
#include "DNest4/code/DNest4.h"
#include "MyModel.h"

using namespace DNest4;

int main(int argc, char** argv)
{
    // Run DNest4
	CommandLineOptions options(argc, argv);
	Sampler<MyModel> sampler = setup<MyModel>(options);
	sampler.run();

    // Get the best particle
    auto best = sampler.get_best_ever_particle();
    std::cout << "Best ever particle = ";
    best.print(std::cout);
    std::cout << std::endl;


    return 0;
}

