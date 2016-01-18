# Import postprocess from two directories up
# http://stackoverflow.com/a/9806045
import os
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
parentdir = os.path.dirname(parentdir)
os.sys.path.insert(0, parentdir)

import dnest4
dnest4.postprocess()

