# distutils: language = c++
from __future__ import division

cimport cython
from libcpp.vector cimport vector
from cython.operator cimport dereference

import numpy as np
cimport numpy as np
np.import_array()

DTYPE = np.float64
ctypedef np.float64_t DTYPE_t


cdef extern from "DNest4.h" namespace "DNest4":

    cdef cppclass Options:
        Options ()
        Options (
            unsigned int num_particles,
            unsigned int new_level_interval,
            unsigned int save_interval,
            unsigned int thread_steps,
            unsigned int max_num_levels,
            double lam,
            double beta,
            unsigned int max_num_saves
        )

    cdef cppclass Sampler[T]:
        Sampler()
        Sampler(unsigned int num_threads, double compression,
                const Options options, unsigned save_to_disk)

        void run()
        void increase_max_num_saves(unsigned int increment)

        vector[T] get_particles()


cdef extern from "PyModel.h":

    cdef cppclass PyModel:
        double m
        double sigma


class State(object):

    pass


def run(
    # "command line" args
    unsigned int seed,
    double compression,

    # sampler args
    unsigned int num_particles,
    unsigned int new_level_interval,
    unsigned int save_interval,
    unsigned int thread_steps,
    unsigned int max_num_levels,
    double lam,
    double beta,
    unsigned int max_num_saves
):

    cdef Options options = Options(
        num_particles, new_level_interval, save_interval, thread_steps,
        max_num_levels, lam, beta, max_num_saves
    )

    cdef Sampler[PyModel] sampler = Sampler[PyModel](1, compression, options, 0)
    cdef vector[PyModel] particles

    cdef int i, j, n
    for i in range(10):
        sampler.run()
        sampler.increase_max_num_saves(1)

        particles = sampler.get_particles()
        n = particles.size()
        result = dict(step=i, particles=[])
        for j in range(n):
            result["particles"].append(dict(
                m=particles[j].m,
                sigma=particles[j].sigma,
            ))

        yield result
