#ifndef DNest4_Normals
#define DNest4_Normals

#include <ostream>
#include "../RNG.h"
#include <vector>

namespace DNest4
{

/* A template class for when I want a vector of quantities
   with a normal prior given some hyperparameters. The
   priors for mu and sigma are independent. */

template<class MuPrior, class SigmaPrior>
class Normals
{
    private:

        // These should be distribution objects
        MuPrior mu_prior;
        SigmaPrior sigma_prior;

        // Values for the hyperparameters
        double mu, sigma;

        // Vectors of latent ns and produced ys
        std::vector<double> ns, ys;

        // Syncing functions
        void sync_logp();   // Requires ys
        void sync_ys();     // Sync ys from ns
        void sync_ns();     // Sync ns from ys

        // log p(y | alpha)
        double logp;

    public:

        // Constructor. Pass priors and size
        Normals(MuPrior _mu_prior,
                SigmaPrior _sigma_prior,
                int size);

        // The usual DNest4 functions
        void from_prior(RNG& rng);
        double perturb(RNG& rng);

        // Size
        size_t size() const;

        // Getter
        const std::vector<double>& get_ys() const;

        // Printer
        void print(std::ostream& out) const;
};

} // namespace

#include "NormalsImpl.h"

#endif

