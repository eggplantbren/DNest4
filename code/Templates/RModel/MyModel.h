#ifndef DNest4_Template_MyModel
#define DNest4_Template_MyModel

#include "DNest4/code/DNest4.h"
#include <ostream>
#include <pthread.h>

/* simple pthread mutex implementation */
class Mutex {
private:
    pthread_mutex_t mutex;

public:
    Mutex() { pthread_mutex_init(&mutex, 0); }

    void lock() { pthread_mutex_lock(&mutex); }
    void unlock() { pthread_mutex_unlock(&mutex); }
};

/* static mutex making sure only one
   thread can enter R at any given point.
   Note that it cannot be a member of MyModel
   since some methods are const */
static Mutex Rmux;

class MyModel
{
    private:

        // Parameter vector with Uniform(0, 1) priors
        std::vector<double> params;

    public:
        // Constructor only gives size of params
        MyModel();

        // Generate the point from the prior
        void from_prior(DNest4::RNG& rng);

        // Metropolis-Hastings proposals
        double perturb(DNest4::RNG& rng);

        // Likelihood function
        double log_likelihood() const;

        // Print to stream
        void print(std::ostream& out) const;

        // Return string with column information
        std::string description() const;
};

#endif

