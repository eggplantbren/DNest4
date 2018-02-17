from pylab import *

# Load the data, plot it, and overplot regression lines
data = loadtxt('road.txt')
posterior_sample = loadtxt('posterior_sample.txt')

xmin = data[:,0].min()
xmax = data[:,0].max()

x = linspace(xmin - 0.2*(xmax - xmin), xmax + 0.2*(xmax - xmin), 1001)

# Plot regression lines
for i in range(posterior_sample.shape[0]):
    y = posterior_sample[i, 0]*x + posterior_sample[i, 1]
    plot(x, y, 'g', alpha=0.01)

plot(data[:,0], data[:,1], 'ko')
xlabel('$x$', fontsize=18)
ylabel('$y$', fontsize=18)
axis([0, 100, 0, 800])
title('Regression Lines', fontsize=16)
savefig('regression_lines.pdf', bbox_inches='tight')
show()

