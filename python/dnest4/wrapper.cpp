#include <iostream>

#include "DNest4.h"
#include "PyModel.h"

using DNest4::Options;
using DNest4::Sampler;

void run_dnest (
    // "command line" args
    unsigned int seed,
    double compression,

    // sampler args
    unsigned int num_particles,
    unsigned int new_level_interval,
    unsigned int save_interval,
    unsigned int thread_steps,
    unsigned int max_num_levels,
    double lambda,
    double beta,
    unsigned int max_num_saves
) {
    // Load sampler options from file
    Options options(
        num_particles, new_level_interval, save_interval,
        thread_steps, max_num_levels, lambda, beta, max_num_saves
    );

    // Create sampler
    Sampler<PyModel> sampler(1.0, compression, options, false);

    // Seed RNGs
    sampler.initialise(seed);

    sampler.run();
}
