#ifndef DNest4_Start
#define DNest4_Start

#include "CommandLineOptions.h"
#include "Sampler.h"

namespace DNest4
{

/*
* Set up the sampler and return it
* You'll still need to call run() on it
*/
template<class ModelType>
Sampler<ModelType> setup(int argc, char** argv);

template<class ModelType>
Sampler<ModelType> setup(const CommandLineOptions& options);

template<class ModelType>
void start(int argc, char** argv);

template<class ModelType>
void start(const CommandLineOptions& options);

} // namespace DNest4

#include "StartImpl.h"
#endif

