from pylab import *

rc("font", size=14, family="serif", serif="Computer Sans")
rc("text", usetex=True)

import dnest4.classic as dn4

posterior_sample = dn4.my_loadtxt('posterior_sample.txt')
data = loadtxt('fake_data.txt')

width = 0.5
bins = np.arange(0.0, 100.0) - 0.5*width

hist(posterior_sample[:,data.shape[0] + 7], bins=bins, width=width, color="k", alpha=0.2)
xlabel('Number of Sinusoids $N$', fontsize=14)
ylabel('Number of posterior samples', fontsize=14)
xlim([-0.5, 100.5])
savefig('N_result.pdf', bbox_inches='tight')
show()

#sample_info = loadtxt('Metropolis/sample_info.txt')
#plot(sample_info[:,1], linewidth=1)
#xlabel('Iteration / 20000')
#ylabel('Log Likelihood')
#savefig('trace_logl.pdf', bbox_inches='tight')
#show()

