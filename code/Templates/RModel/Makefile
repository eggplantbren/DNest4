# Put the directory above 'DNest4' into an environment variable
# called DNEST4_PATH for this to work.

CC = g++

# Path to R.h
R_PATH = /usr/include/R/

# Path to Rcpp.h
RCPP_PATH = /home/brewer/R/x86_64-pc-linux-gnu-library/3.5/Rcpp/include/

# Path to RInside.h
RINSIDE_PATH = /home/brewer/R/x86_64-pc-linux-gnu-library/3.5/RInside/include/

# Path to libR.so
RLIB_PATH = /usr/lib/R/lib/

# Path to libRInside.so
RINSIDE_LIB_PATH = /home/brewer/R/x86_64-pc-linux-gnu-library/3.5/RInside/lib/

CXXFLAGS = -std=c++11 -O3 -march=native -DNDEBUG
LIBS = -ldnest4 -lpthread -lR -lRInside

default:
	$(CC) -I$(DNEST4_PATH) -I$(R_PATH) -I$(RINSIDE_PATH) -I$(RCPP_PATH) $(CXXFLAGS) -c *.cpp
	$(CC) -pthread -L$(DNEST4_PATH)/DNest4/code -L$(RLIB_PATH) -L$(RINSIDE_LIB_PATH) -o main *.o $(LIBS)
	rm *.o

