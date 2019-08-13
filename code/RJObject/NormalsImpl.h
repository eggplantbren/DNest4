#include <cassert>
#include <cmath>

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
void Normals<MuPrior, SigmaPrior>::sync_logp()
{
    double C = -0.5*log(2.0*M_PI*sigma*sigma);
    double tau = 1.0/(sigma*sigma);
    logp = 0.0;
    for(double y: ys)
        logp += C - 0.5*tau*pow(y - mu, 2);
}



} // namespace

