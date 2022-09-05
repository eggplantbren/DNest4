import corner
import dnest4.classic as dn4
import matplotlib.pyplot as plt
import numpy as np

# DNest4 postprocessing
dn4.postprocess(cut=0.0)

# Corner plot
posterior_sample = dn4.my_loadtxt("posterior_sample.txt")
corner.corner(posterior_sample, plot_contours=False, plot_density=False)
plt.show()

# Correlation matrix
plt.imshow(np.corrcoef(posterior_sample.T), cmap="coolwarm",
           vmin=-1.0, vmax=1.0, interpolation="nearest")
plt.show()

