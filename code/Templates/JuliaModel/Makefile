# Put the directory above 'DNest4' into an environment variable
# called DNEST4_PATH for this to work.

CC = g++
CXXFLAGS = -std=c++11 -O3 -march=native -Wall -Wextra -DNDEBUG
LIBS = -ldnest4 -lpthread -ljulia

default:
	$(CC) -I$(DNEST4_PATH) -I$(JULIA_INCLUDE_PATH) $(CXXFLAGS) -c *.cpp
	$(CC) -pthread -L$(JULIA_LIB_PATH) -L$(DNEST4_PATH)/DNest4/code -o main *.o $(LIBS)
	rm *.o

