# Put the directory above 'DNest4' into an environment variable
# called DNEST4_PATH for this to work.

CC = g++
JULIA_LIB_PATH=/home/brewer/.julia/juliaup/julia-1.11.5+0.x64.linux.gnu/lib
JULIA_INCLUDE_PATH=/home/brewer/.julia/juliaup/julia-1.11.5+0.x64.linux.gnu/include/julia
CXXFLAGS = -std=c++11 -O3 -march=native -Wall -Wextra -DNDEBUG -DNO_THREADS
LIBS = -ldnest4 -lpthread -ljulia

default:
	$(CC) -I$(DNEST4_PATH) -I$(JULIA_INCLUDE_PATH) $(CXXFLAGS) -c *.cpp
	$(CC) -Wl,-rpath,$(JULIA_LIB_PATH) -pthread -L$(JULIA_LIB_PATH) -L$(DNEST4_PATH)/DNest4/code -o main *.o $(LIBS)
	rm *.o

