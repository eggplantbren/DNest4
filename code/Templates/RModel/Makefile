# Put the directory above 'DNest4' into an environment variable
# called DNEST4_PATH for this to work.

CC = g++

# Path to R.h
R_PATH = /usr/share/R/include/

# Path to Rcpp.h
RCPP_PATH = /usr/lib/R/site-library/Rcpp/include/

# Path to RInside.h
RINSIDE_PATH = /usr/local/lib/R/site-library/RInside/include/

# Path to libR.so
RLIB_PATH = /usr/lib/R/lib/

# Path to libRInside.so
# You will also need to add this to your LD_LIBRARY_PATH
RINSIDE_LIB_PATH = /usr/local/lib/R/site-library/RInside/lib/

CXXFLAGS = -std=c++11 -O3 -march=native -DNDEBUG
LIBS = -ldnest4 -lpthread -lR -lRInside

default:
	$(CC) -I$(DNEST4_PATH) -I$(R_PATH) -I$(RINSIDE_PATH) -I$(RCPP_PATH) $(CXXFLAGS) -c *.cpp
	$(CC) -pthread -L$(DNEST4_PATH)/DNest4/code -L$(RLIB_PATH) -L$(RINSIDE_LIB_PATH) -o main *.o $(LIBS)
	rm *.o

