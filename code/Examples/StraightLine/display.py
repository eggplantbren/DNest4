from pylab import *

# Load the data, plot it, and overplot regression lines
data = loadtxt('road.txt')
posterior_sample = loadtxt('posterior_sample.txt')

xmin = data[:,0].min()
xmax = data[:,0].max()

x = linspace(xmin - 0.2*(xmax - xmin), xmax + 0.2*(xmax - xmin), 1001)
ion()

for i in range(posterior_sample.shape[0]):
	hold(False)
	plot(data[:,0], data[:,1], 'bo')
	hold(True)
	y = posterior_sample[i, 0]*x + posterior_sample[i, 1]
	plot(x, y, 'k')
	xlabel('$x$', fontsize=20)
	ylabel('$y$', fontsize=20)
	title('Model {k}/{N}'.format(k=(i+1), N=posterior_sample.shape[0]))
	draw()
ioff()
show()

