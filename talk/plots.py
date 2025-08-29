import numpy as np
import numpy.random as rng
import matplotlib.pyplot as plt

rng.seed(0)

plt.rcParams.update({
    "text.usetex": True,
    "font.size": 14,
})


theta = np.linspace(-5.0, 5.0, 1001)
prior = 1.0/(1.0 + (theta - 1.0)**2)
prior = prior/np.trapz(prior, x=theta)
likelihood = 0.3*np.exp(-0.5*(theta + 1.5)**2)
posterior = prior*likelihood
posterior /= np.trapz(posterior, x=theta)

plt.plot(theta, prior, color="black", linewidth=2, label="Prior")
plt.plot(theta, likelihood, color="blue", alpha=0.5, label="Likelihood")
plt.plot(theta, posterior, color="red", linewidth=2, label="Posterior")
plt.legend()
plt.xlabel("Unknown Parameter $\\theta$")
plt.ylim(0.0)
plt.savefig("bayes.pdf", bbox_inches="tight")

