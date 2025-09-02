#ifndef DNest4_Hypercube_h
#define DNest4_Hypercube_h

#include "RNG.h"
#include "Utils.h"
#include <iostream>
#include <vector>

namespace DNest4
{

// Base class for hypercube-based models.
template<int size, class T>
class Hypercube
{
    protected:
        std::vector<double> us, xs;

    public:
        Hypercube();
        void from_prior(DNest4::RNG& rng);
        double perturb(DNest4::RNG& rng);
        void print(std::ostream& out) const;
};


/* Implementations follow */

template<int size, class T>
Hypercube<size, T>::Hypercube()
:us(size)
,xs(size)
{

}

template<int size, class T>
void Hypercube<size, T>::from_prior(DNest4::RNG& rng)
{
    for(double& u: us)
        u = rng.rand();

    static_cast<T*>(this)->prior_transform();
}


template<int size, class T>
double Hypercube<size, T>::perturb(DNest4::RNG& rng)
{
    int reps;
    if(rng.rand() <= 0.5)
        reps = 1;
    else
        reps = (int)pow(size, rng.rand());

    for(int i=0; i<reps; ++i)
    {
        int k = rng.rand_int(us.size());
        us[k] += rng.randh();
        DNest4::wrap(us[k], 0.0, 1.0);
    }

    static_cast<T*>(this)->prior_transform();

    return 0.0;
}


template<int size, class T>
void Hypercube<size, T>::print(std::ostream& out) const
{
    for(double x: xs)
        out << x << ' ';
}

} // namespace

#endif

