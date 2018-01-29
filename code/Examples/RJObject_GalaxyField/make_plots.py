from pylab import *
import dnest4.classic as dn4

rc("font", size=14, family="serif", serif="Computer Sans")
rc("text", usetex=True)

# Piecewise linear stretch
def stretch(x):
	y = x.copy()
	y = (y - y.min())/(y.max() - y.min())
	y[y > 0.1] = 0.1 + 0.05*(y[y > 0.1] - 0.1)
	return y

data = dn4.my_loadtxt('Data/test_image.txt')

img = stretch(data)

imshow(img, cmap='gray', interpolation='nearest')
xlabel('$x$')
ylabel('$y$')
gca().set_xticks([-0.5, 99.5, 199.5])
gca().set_yticks([-0.5, 99.5, 199.5])
gca().set_xticklabels(['-1', '0', '1'])
gca().set_yticklabels(['1', '0', '-1'])
title('GalaxyField Data')
savefig('galaxyfield_data.pdf', bbox_inches='tight')
show()

posterior_sample = dn4.my_loadtxt('posterior_sample.txt')

hist(posterior_sample[:,40006], 50)
xlabel('Number of Galaxies $N$', fontsize=14)
ylabel('Number of posterior samples', fontsize=14)
xlim([-0.5, 100.5])
savefig('N_galaxy_result.pdf', bbox_inches='tight')
show()

