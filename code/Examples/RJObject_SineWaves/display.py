from pylab import *
import os
import dnest4.classic as dn4

data = loadtxt('fake_data.txt')
posterior_sample = atleast_2d(dn4.my_loadtxt('posterior_sample.txt'))

saveFrames = False # For making movies
if saveFrames:
  os.system('rm Frames/*.png')

ion()
for i in range(0, posterior_sample.shape[0]):
  hold(False)
  plot(data[:,0], data[:,1], 'k.', alpha=0.2)
  hold(True)
  plot(data[:,0], posterior_sample[i, 0:data.shape[0]], 'g')
  xlabel('Time', fontsize=16)
  ylabel('y', fontsize=16)
  draw()
  if saveFrames:
    savefig('Frames/' + '%0.4d'%(i+1) + '.png', bbox_inches='tight')
    print('Frames/' + '%0.4d'%(i+1) + '.png')

ioff()
show()
