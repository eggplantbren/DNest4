import numpy as np
from matplotlib import rc
import matplotlib.pyplot as plt

"""
Make a plot of p(theta, D)
"""

plt.rc("font", size=18, family="serif", serif="Computer Sans")
plt.rc("text", usetex=True)

# Resolution
N = 256
[x, y] = np.meshgrid(np.linspace(0., 5., N), np.linspace(5., 0., N))

f = np.exp(-0.5*(x-2.5)**2/1.**2)*np.exp(-0.5*((y - 5*(x/5)**2)**2)/0.3**2)
f /= f.sum()

plt.imshow(f, extent=[x.min(), x.max(), y.min(), y.max()], cmap='Blues')
plt.xlabel(r'$\theta$')
plt.ylabel(r'$D$')
plt.axhline(1.3, color='k')
plt.axhline(1.5, color='k')
plt.fill_between(x[0, :], 1.3, 1.5, color=[0.2, 0.2, 0.2], alpha=0.2)
plt.fill_between(x[0, :], 1.2, 1.8, color=[0.6, 0.6, 0.6], alpha=0.2)
plt.savefig('figures/joint.pdf', bbox_inches='tight')
#plt.show()

