# Put the directory above 'DNest4' into an environment variable
# called DNEST4_PATH for this to work.

CC = g++

PYTHON_PATH=/usr/include/python3.10
CXXFLAGS = -std=c++11 -O3 -DNO_THREADS
LIBS = -ldnest4 -lpthread -lpython3.10

default:
	$(CC) -I pybind11/include -I$(DNEST4_PATH) -I$(PYTHON_PATH) $(CXXFLAGS) -c *.cpp
	$(CC) -pthread -L$(DNEST4_PATH)/DNest4/code -o main *.o $(LIBS)
	rm *.o

