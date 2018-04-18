CC = g++
CXXFLAGS = -std=c++11 -O3 -march=native -Wall -Wextra -pedantic -DNDEBUG
LIBS = -ldnest4 -lpthread

default:
	$(CC) -I ../../../.. $(CXXFLAGS) -c *.cpp
	$(CC) -pthread -L$(DNEST4_PATH)/DNest4/code -o main *.o $(LIBS)
	rm *.o

