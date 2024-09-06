# Put the directory above 'DNest4' into an environment variable
# called DNEST4_PATH for this to work.

CC = g++

# Path to R.h
R_PATH = /usr/share/R/include/

# Path to libR.so
RLIB_PATH = /usr/lib/R/lib/

CXXFLAGS = -std=c++11 -O3 -march=native -DNDEBUG
LIBS = -ldnest4 -lpthread -lR

default:
	$(CC) -I$(DNEST4_PATH) -I$(R_PATH) -I$(RINSIDE_PATH) $(CXXFLAGS) -c *.cpp
	$(CC) -pthread -L$(DNEST4_PATH)/DNest4/code -L$(RLIB_PATH) -o main *.o $(LIBS)
	rm *.o

