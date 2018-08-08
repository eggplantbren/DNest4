import dnest4.classic as dn4
import os
import matplotlib.pyplot
from pylab import *

print("This script should \
replicate all the runs presented in the paper.\nNote:\
plots will be generated, which need to be manually\n\
closed for the script to continue. It assumes\n\
everything has been compiled already as per the instructions in the paper.\n\
\n\
NOTE: the RJObject_1DMixture example is fairly time-consuming.\n\n\
Press ENTER to begin.")
junk = input()

def run_example(directory, abc=False):
    os.chdir(directory)
    os.system("./main -s 0 -t 8")
    if directory == "StraightLine":
        print("Figure 1 in the paper has been zoomed in.")

    if abc:
        dn4.postprocess_abc(rng_seed=0)
    else:
        dn4.postprocess(rng_seed=0)

    exec(open("display.py").read())
    os.chdir("..")

os.chdir("../code/Examples")
run_example("StraightLine")
run_example("RJObject_1DMixture")
run_example("ABC", abc=True)

