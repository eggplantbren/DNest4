#ifndef DNest4_TwinPeaksModel
#define DNest4_TwinPeaksModel

#include <ostream>
#include <vector>
#include "DNest4/code/RNG.h"

class TwinPeaksModel
{
    private:
        static constexpr int N = 100;
        std::vector<double> xs;

    public:
        TwinPeaksModel();

        void from_prior(DNest4::RNG& rng);
        double perturb(DNest4::RNG& rng);
        void print(std::ostream& out) const;

        // The two scalars
        double f() const;
        double g() const;

};

#endif

