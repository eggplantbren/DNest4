import dnest4.classic as dn4
import matplotlib.pyplot as plt
import numpy as np
import os

sample = dn4.my_loadtxt("sample.txt")
num_atoms = sample.shape[1] // 3
x = sample[:, 0:num_atoms]
y = sample[:, num_atoms:2*num_atoms]

os.system("rm figures/*.png")
for i in range(0, sample.shape[0]):
    plt.clf()
    plt.plot(x[i, :], y[i, :], ".")
    plt.axis("square")
    plt.axis([0.0, 50.0, 0.0, 50.0])
    plt.savefig("figures/{i}.png".format(i=i))
    print(i+1)

