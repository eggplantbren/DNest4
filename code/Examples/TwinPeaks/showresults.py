import matplotlib.pyplot as plt
import numpy as np
import dnest4.classic as dn4

dn4.postprocess()
log_prior_weights = np.loadtxt("log_prior_weights.txt")

# Look at a canonical distribution
temperatures = [30.0, 1.0]
sample = dn4.my_loadtxt("sample.txt")
fs, gs = sample[:,-2], sample[:,-1]

# Calculate logZ
F = fs/temperatures[0] + gs/temperatures[1]
logZ = dn4.logsumexp(log_prior_weights + F)
logP = log_prior_weights + F - logZ
P = np.exp(logP - logP.max())
P /= P.sum()
H = np.sum(P*(F - logZ))
print("")
print("logZ of canonical distribution =", logZ)
print("H of canonical distribution =", H)

# True logZ
x = np.linspace(0.0, 1.0, 10001)
y = np.exp(-0.5*(x - 0.5)**2 / 0.01**2 / temperatures[0] + \
            np.cos(2.0*np.pi*(x - 0.5)*10.0) / temperatures[1])
p = y / np.trapz(y, x=x)
print("")
print("True logZ =", 100*np.log(np.trapz(y, x=x)))
print("True H =", 100*np.trapz(p*np.log(p + 1E-300), x=x))
plt.plot(x, y)
plt.title("Canonical distribution")
plt.show()


# Plot scalars

plt.plot(fs, gs, ".", markersize=3, alpha=0.3)
plt.show()

