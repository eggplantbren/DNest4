import os
import matplotlib.pyplot

# Best Python command on your system
# (should also be the one you installed the dnest4 package for)
my_python = "python"

print("This script should\
replicate all the runs presented in the paper.\nNote:\
plots will be generated, which need to be manually\n\
closed for the script to continue. It assumes\n\
everything has been compiled already as per the instructions in the paper.\n\
\n\
NOTE: the RJObject_1DMixture example is fairly time-consuming.\n\n\
Press ENTER to begin.")
junk = input()

def run_example(directory):
    os.chdir(directory)
    os.system("./main -s 0")
    if directory == "StraightLine":
        print("Figure 1 in the paper has been zoomed in.")
    os.system(my_python + " showresults.py")
    os.chdir("..")

os.chdir("../code/Examples")
run_example("StraightLine")
run_example("RJObject_1DMixture")
run_example("ABC")

