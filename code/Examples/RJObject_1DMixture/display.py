from pylab import *
import dnest4.classic as dn4
import os

data = loadtxt('galaxies.txt')
posterior_sample = atleast_2d(dn4.my_loadtxt('posterior_sample.txt'))

os.system("mkdir Frames")
os.system("rm Frames/*.png")

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

total = zeros(x.shape)

for i in range(0, posterior_sample.shape[0]):
    hold(False)
    hist(data, 100, alpha=0.5, normed=True)
    hold(True)
    y = mixture(x, posterior_sample[i, :])
    total += y
    plot(x, y, 'r', linewidth=2)
    title("{a} / {b}".format(a=(i+1), b=posterior_sample.shape[0]))
    savefig('Frames/' + '%0.6d'%(i+1) + '.png', bbox_inches='tight')
    print('Frames/' + '%0.6d'%(i+1) + '.png')

show()

hist(data, 100, alpha=0.5, normed=True)
hold(True)
plot(x, total/posterior_sample.shape[0], 'r', linewidth=2)
show()

