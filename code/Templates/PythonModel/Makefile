# Put the directory above 'DNest4' into an environment variable
# called DNEST4_PATH for this to work.

CC = g++

CXXFLAGS = -std=c++11 -O3 -DNO_THREADS
LIBS = -ldnest4 -lpthread -lpython3.10

default:
	$(CC) -I$(DNEST4_PATH) -I/usr/include/python3.10/ $(CXXFLAGS) -c *.cpp
	$(CC) -pthread -L$(DNEST4_PATH)/DNest4/code -L$(RLIB_PATH) -L$(RINSIDE_LIB_PATH) -o main *.o $(LIBS)
	rm *.o

