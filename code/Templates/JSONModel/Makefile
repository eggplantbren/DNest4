# Put the directory above 'DNest4' into an environment variable
# called DNEST4_PATH for this to work.

CC = g++

CXXFLAGS = -std=c++11 -O3
LIBS = -ldnest4 -lpthread

default:
	$(CC) -I$(DNEST4_PATH) -I$(PYTHON_PATH) $(CXXFLAGS) -c *.cpp
	$(CC) -pthread -L$(DNEST4_PATH)/DNest4/code -o main *.o $(LIBS)
	rm *.o

