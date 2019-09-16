import dnest4.classic as dn4
import matplotlib.pyplot as plt
import numpy as np

def canonical(temperatures,
                    log_prior_weights,
                    samples, verbose=True):
    """
    Compute properties of a canonical distribution
    """

    # Calculate logZ using the three different available sets of
    # log-prior-weights and return the highest

    logZ = []
    for k in range(3):
        lpw, sample = log_prior_weights[k], samples[k]
        fs, gs = sample[:,-2], sample[:,-1]

        F = fs/temperatures[0] + gs/temperatures[1]
        logZ.append(dn4.logsumexp(lpw + F))

    highest = int(np.nonzero(logZ == np.max(logZ))[0][0])
    log_prior_weights = log_prior_weights[highest]
    logZ = logZ[highest]

    logP = log_prior_weights + F - logZ
    P = np.exp(logP - logP.max())
    P /= P.sum()
    H = np.sum(P*(F - logZ))

    if verbose:
        print("")
        print("logZ of canonical distribution =", logZ)
        print("H of canonical distribution =", H)

    return {"logZ": logZ, "H": H, "logP": logP}



def truth(temperatures, verbose=True):
    """
    Compute properties of a canonical distribution
    """
    x = np.linspace(0.0, 1.0, 10001)
    y = np.exp(-0.5*(x - 0.5)**2 / 0.01**2 / temperatures[0] + \
                np.cos(2.0*np.pi*(x - 0.5)*10.0) / temperatures[1])
    p = y / np.trapz(y, x=x)
    logZ = 100*np.log(np.trapz(y, x=x))
    H = 100*np.trapz(p*np.log(p + 1E-300), x=x)

    if verbose:
        print("True logZ =", logZ)
        print("True H =", H)

    return { "logZ": logZ, "H": H }




def grid(log_prior_weights, samples):
    """
    Analyse a temperature grid
    """
    T1 = np.exp(np.linspace(np.log(0.1), np.log(100.0), 31))
    T2 = np.exp(np.linspace(np.log(0.1), np.log(100.0), 31))
    [T1, T2] = np.meshgrid(T1, T2[::-1])

    logZ = np.empty(T1.shape)
    true_logZ = np.empty(T1.shape)
    for i in range(T1.shape[0]):
        for j in range(T1.shape[1]):
            Ts = [T1[i, j], T2[i, j]]
            result = canonical(Ts, log_prior_weights, samples, verbose=False)
            logZ[i, j] = result["logZ"]
            true_logZ[i, j] = truth(Ts, verbose=False)["logZ"]
        print(".", end="", flush=True)

    print("")
    plt.imshow(logZ - true_logZ, cmap="coolwarm")
    plt.show()

    return logZ


if __name__ == "__main__":

    log_prior_weights = [np.loadtxt("log_prior_weights_first.txt"),
                         np.loadtxt("log_prior_weights_second.txt"),
                         np.loadtxt("log_prior_weights_final.txt")]

    samples = [dn4.my_loadtxt("sample_first.txt"),
               dn4.my_loadtxt("sample_second.txt"),
               dn4.my_loadtxt("sample_final.txt")]

    temperatures = [30.0, 1.0]
    canonical(temperatures, log_prior_weights, samples)
    truth(temperatures)
    grid(log_prior_weights, samples)


## True logZ

#plt.plot(x, y)
#plt.title("Canonical distribution")
#plt.show()



