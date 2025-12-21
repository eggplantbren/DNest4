#ifndef DNest4_Hypercube_h
#define DNest4_Hypercube_h

#include "RNG.h"
#include "Utils.h"
#include <iostream>
#include <string>
#include <vector>

namespace DNest4
{

// Base class for hypercube-based models.
template<class T>
class Hypercube
{
    protected:
        static int size;
        std::vector<double> us;

    public:
        static void set_size(int _size);

        Hypercube();
        void from_prior(DNest4::RNG& rng);
        double perturb(DNest4::RNG& rng);
        void print(std::ostream& out) const;
        virtual std::string description() const;
};


/* Implementations follow */
template<class T>
int Hypercube<T>::size = 1;

template<class T>
void Hypercube<T>::set_size(int _size)
{
    size = _size;
}

template<class T>
Hypercube<T>::Hypercube()
:us(size)
{

}

template<class T>
void Hypercube<T>::from_prior(DNest4::RNG& rng)
{
    for(double& u: us)
        u = rng.rand();

    static_cast<T*>(this)->prior_transform();
}


template<class T>
double Hypercube<T>::perturb(DNest4::RNG& rng)
{
    perturb_us(us, rng);

    static_cast<T*>(this)->prior_transform();

    return 0.0;
}


template<class T>
void Hypercube<T>::print(std::ostream& out) const
{
    static_cast<T*>(this)->print(out);
}

template<class T>
std::string Hypercube<T>::description() const
{
    return std::string("");
}


} // namespace

#endif

