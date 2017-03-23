import dnest4.classic as dn4
dn4.postprocess(cut=0.0)

# Plots from the blog post
import matplotlib.pyplot as plt
import numpy as np

posterior_sample = dn4.my_loadtxt("posterior_sample.txt")
plt.plot(posterior_sample[:,41], posterior_sample[:,42],
         "k.", markersize=1, alpha=0.2)
plt.xlabel("$x_{41}$")
plt.ylabel("$x_{42}$")
plt.show()

plt.imshow(np.corrcoef(posterior_sample.T), cmap="coolwarm",
           vmin=-1.0, vmax=1.0, interpolation="nearest")
plt.show()

