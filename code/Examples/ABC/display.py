import matplotlib.pyplot as plt
import numpy as np
import dnest4.classic as dn4

plt.rcParams["font.family"] = "serif"
plt.rcParams["font.size"] = 16
plt.rc("text", usetex=True)

posterior_sample = np.atleast_2d(dn4.my_loadtxt("posterior_sample.txt"))
plt.plot(posterior_sample[:,0], np.exp(posterior_sample[:,1]),\
                    "ko", alpha=0.1, markersize=5, label="Posterior samples")
plt.plot(0, 1, "r*", markersize=20, label="Truth", alpha=0.5)
plt.xlabel("$\\mu$")
plt.ylabel("$\\sigma$")
plt.legend(loc="upper left", numpoints=1)
plt.title("ABC Results")
plt.axis([-1.5, 1.5, 0.5, 1.5])
plt.savefig("abc_results.pdf", bbox_inches="tight")
plt.show()

