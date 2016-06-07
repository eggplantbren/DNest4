from pylab import *
import dnest4.classic as dn4

rc("font", size=18, family="serif", serif="Computer Sans")
rc("text", usetex=True)

data = loadtxt('galaxies.txt')
posterior_sample = atleast_2d(dn4.my_loadtxt('posterior_sample.txt'))

x = linspace(0., 50.0, 10001)
def mixture(x, params):
    N = int(params[7])
    centers = params[8:108][0:N]
    widths = exp(params[108:208][0:N])
    weights = exp(params[208:308][0:N])
    weights /= weights.sum()

    y = zeros(x.shape)
    for i in range(0, N):
        y += weights[i]/widths[i]/sqrt(2.*pi)*exp(-0.5*(x - centers[i])**2/widths[i]**2)

    return y


hold(False)
hist(data, 100, alpha=0.2, color="k", normed=True)
y_tot = zeros(len(x))

for i in range(0, posterior_sample.shape[0]):
    y = mixture(x, posterior_sample[i, :])
    y_tot += y

hold(True)
plot(x, y_tot/posterior_sample.shape[0], 'g', linewidth=2)

xlabel("Velocity (1000 km/s)")
ylabel("Density")
savefig("galaxies.pdf", bbox_inches="tight")
show()

width = 0.3
bins = arange(0, 101) - 0.5*width
hist(posterior_sample[:,7], bins, width=width, normed=True, color="k", alpha=0.2)
xlim([0, 15])
ylim([0, 0.5])
xlabel("Number of gaussians, $N$")
ylabel("Posterior Probability")
savefig("galaxies_N.pdf", bbox_inches="tight")
show()

