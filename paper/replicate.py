import os

# Python command on your system
my_python = "/home/brendon/local/anaconda3/bin/python"

print("This script should replicate all the runs presented in the paper.\nNote:\
plots will be generated, which need to be manually\n\
closed for the script to continue. It assumes\n\
everything has been compiled already.\n\
\n\
NOTE: the RJObject_1DMixture example is fairly time-consuming.\n\n\
Press ENTER to begin.")
junk = input()

os.system("wget https://github.com/eggplantbren/DNest4/archive/0.1.0.tar.gz")
os.system("tar xvzf 0.1.0.tar.gz")
os.system("mv DNest4-0.1.0 DNest4")
os.chdir("DNest4/code")
os.system("make")
os.chdir("../python")
os.system("python setup.py install")

def run_example(directory):
    os.chdir(directory)
    os.system("./main -s 0")
    os.system(my_python + " showresults.py")
    os.chdir("..")

os.chdir("../code/Examples")
run_example("StraightLine")
run_example("RJObject_1DMixture")
run_example("ABC")

