#include <cassert>
#include <cmath>
#include "../Utils.h"

namespace DNest4
{

template<class MuPrior, class SigmaPrior>
Normals<MuPrior, SigmaPrior>::Normals(MuPrior _mu_prior,
                                      SigmaPrior _sigma_prior,
                                      int size)
:mu_prior(std::move(_mu_prior))
,sigma_prior(std::move(_sigma_prior))
,ns(size)
,ys(size)
{
    assert(size >= 1);
}


template<class MuPrior, class SigmaPrior>
void Normals<MuPrior, SigmaPrior>::from_prior(RNG& rng)
{
    mu = mu_prior.generate(rng);
    sigma = sigma_prior.generate(rng);

    for(double& n: ns)
        n = rng.randn();

    sync_ys();
    sync_logp();
}

template<class MuPrior, class SigmaPrior>
void Normals<MuPrior, SigmaPrior>::sync_ys()
{
    for(size_t i=0; i<ns.size(); ++i)
        ys[i] = mu + sigma*ns[i];
}

template<class MuPrior, class SigmaPrior>
void Normals<MuPrior, SigmaPrior>::sync_ns()
{
    double inv_sigma = 1.0/sigma;
    for(size_t i=0; i<ns.size(); ++i)
        ns[i] = inv_sigma*(ys[i] - mu);
}


template<class MuPrior, class SigmaPrior>
void Normals<MuPrior, SigmaPrior>::sync_logp()
{
    double C = -0.5*log(2.0*M_PI*sigma*sigma);
    double tau = 1.0/(sigma*sigma);
    logp = 0.0;
    for(double y: ys)
        logp += C - 0.5*tau*pow(y - mu, 2);
}

template<class MuPrior, class SigmaPrior>
double Normals<MuPrior, SigmaPrior>::perturb(RNG& rng)
{
    double logH = 0.0;

    int which = rng.rand_int(3);
    if(which == 0)
    {
        // Perturb a hyperparameter, moving all ys
        if(rng.rand() <= 0.5)
            logH += mu_prior.perturb(mu, rng);
        else
            logH += sigma_prior.perturb(sigma, rng);
        sync_ys();
        sync_logp();
    }
    else if(which == 1)
    {
        // Perturb a hyperparameter, keeping ys fixed
        logH -= logp;
        if(rng.rand() <= 0.5)
            logH += mu_prior.perturb(mu, rng);
        else
            logH += sigma_prior.perturb(sigma, rng);
        sync_logp();
        logH += logp;
        sync_ns();
    }
    else
    {
        // Just perturb ns
        logH += perturb_ns(ns, rng);
        sync_ys();
        sync_logp();
    }

    return logH;
}


// Getter
template<class MuPrior, class SigmaPrior>
const std::vector<double>& Normals<MuPrior, SigmaPrior>::get_ys() const
{
    return ys;
}


// Printer
template<class MuPrior, class SigmaPrior>
void Normals<MuPrior, SigmaPrior>::print(std::ostream& out) const
{
    out << mu << ' ' << sigma << ' ';
    for(double y: ys)
        out << y << ' ';
}

template<class MuPrior, class SigmaPrior>
size_t Normals<MuPrior, SigmaPrior>::size() const
{
    return ys.size();
}


} // namespace

