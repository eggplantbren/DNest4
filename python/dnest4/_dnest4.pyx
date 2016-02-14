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

        void initialise(unsigned int first_seed)
        void run()
        void increase_max_num_saves(unsigned int increment)

        vector[T] get_particles()
        int size()
        T* particle(unsigned int i)


cdef extern from "PyModel.h":

    cdef cppclass PyModel:
        void set_py_self (object py_self)
        object get_py_self ()
        int get_exception ()
        object get_npy_coords ()


def run(
    self,

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
):
    cdef int i, j, n, error
    cdef Options options = Options(
        num_particles, new_level_interval, save_interval, thread_steps,
        max_num_levels, lam, beta, 1
    )

    cdef Sampler[PyModel] sampler = Sampler[PyModel](1, compression, options, 0)
    cdef PyModel* particle

    # Initialize the particles.
    n = sampler.size()
    for j in range(n):
        particle = sampler.particle(j)
        particle.set_py_self(self)
        error = particle.get_exception()
        if error != 0:
            raise DNest4Error(error)

    # Initialize the sampler.
    sampler.initialise(seed)
    n = sampler.size()
    for j in range(n):
        particle = sampler.particle(j)
        error = particle.get_exception()
        if error != 0:
            raise DNest4Error(error)

    for i in range(10):
        sampler.run()

        # Loop over particles, build the results list, and check for errors.
        n = sampler.size()
        result = dict(step=i, particles=[])
        for j in range(n):
            # Errors?
            particle = sampler.particle(j)
            error = particle.get_exception()
            if error != 0:
                raise DNest4Error(error)

            # Results.
            result["particles"].append(particle.get_npy_coords())

        # Yield items as a generator.
        result["particles"] = np.array(result["particles"])
        yield result

        # Hack to continue running.
        sampler.increase_max_num_saves(1)


class DNest4Error(Exception):
    pass
