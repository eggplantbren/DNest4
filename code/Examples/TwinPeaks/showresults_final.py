import matplotlib.pyplot as plt
import numpy as np
import dnest4.classic as dn4


def canonical(temperatures,
                    log_prior_weights = [np.loadtxt("log_prior_weights_first.txt"),
                     np.loadtxt("log_prior_weights_second.txt"),
                     np.loadtxt("log_prior_weights_final.txt")],
                    samples = [dn4.my_loadtxt("sample_first.txt"),
                               dn4.my_loadtxt("sample_second.txt"),
                               dn4.my_loadtxt("sample_final.txt")]):
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
        plt.plot(lpw)
        plt.plot(lpw + F)
        plt.show()

    highest = int(np.nonzero(logZ == np.max(logZ))[0][0])
    log_prior_weights = log_prior_weights[highest]
    logZ = logZ[highest]

    logP = log_prior_weights + F - logZ
    P = np.exp(logP - logP.max())
    P /= P.sum()
    H = np.sum(P*(F - logZ))
    print("")
    print("logZ of canonical distribution =", logZ)
    print("H of canonical distribution =", H)
    return {"logZ": logZ, "H": H, "logP": logP}



def truth(temperatures):
    """
    Compute properties of a canonical distribution
    """
    x = np.linspace(0.0, 1.0, 10001)
    y = np.exp(-0.5*(x - 0.5)**2 / 0.01**2 / temperatures[0] + \
                np.cos(2.0*np.pi*(x - 0.5)*10.0) / temperatures[1])
    p = y / np.trapz(y, x=x)
    print("True logZ =", 100*np.log(np.trapz(y, x=x)))
    print("True H =", 100*np.trapz(p*np.log(p + 1E-300), x=x))

    plt.plot(x, y)
    plt.show()

temperatures = [30.0, 1.0]
canonical(temperatures)
truth(temperatures)



## True logZ

#plt.plot(x, y)
#plt.title("Canonical distribution")
#plt.show()



