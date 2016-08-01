from pylab import *

seed(123)

"""
This one has a phase transition at temperature = 1 :-D
"""

t = linspace(0., 100., 1001)
y = 7*sin(2.*pi*t/30.) + 0.155*sin(2.*pi*t/2. + 1.)
sig = 0.5
Y = y + sig*randn(y.size)

data = empty((t.size, 3))
data[:,0], data[:,1] = t, Y
data[:,2] = 0.0

savetxt('fake_data.txt', data)

rc("font", size=14, family="serif", serif="Computer Sans")
rc("text", usetex=True)
plot(t, y, 'r', linewidth=2, label='Noise-free signal')
plot(t, Y, 'bo', markersize=3, alpha=0.5, label='Noisy measurements')
ylim([-12., 13.])
legend(numpoints=1)
xlabel('Time')
ylabel('Signal')
savefig('sinewave_data.pdf', bbox_inches='tight')
show()

