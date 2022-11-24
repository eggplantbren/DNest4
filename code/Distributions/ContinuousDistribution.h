#ifndef DNest4_Distribution
#define DNest4_Distribution

#include <iostream>
#include "../RNG.h"

namespace DNest4
{

/*
* An abstract base class for continuous probability distributions.
*/
class ContinuousDistribution
{
    private:

    public:
        double generate(RNG& rng) const;
        double perturb(double& x, RNG& rng) const;

        virtual void setpars(double) {};
        virtual void setpars(double, double) {};

        virtual double cdf(double x) const = 0;
        virtual double cdf_inverse(double x) const = 0;
        virtual double log_pdf(double x) const = 0;

        // overload operator<<
	    friend std::ostream& operator<<(std::ostream &out, const ContinuousDistribution &b)
        {
            // delegate printing to member function print()
            return b.print(out);
        };
        // member function print() will do the actual printing
        // because it is a normal member function, it can be virtualized
        virtual std::ostream& print(std::ostream& out) const {return out;};

};

} // namespace DNest4

#endif

