CC = g++
CXXFLAGS = -std=c++11 -O3 -Wall -Wextra -pedantic -DNDEBUG
LIBS = -ldnest4 -lpthread

default:
	make noexamples -C ../..
	$(CC) -I ../../../.. $(CXXFLAGS) -c *.cpp
	$(CC) -L ../.. -o main *.o $(LIBS)
	rm *.o

nolib:
	$(CC) -I ../../../.. $(CXXFLAGS) -c *.cpp
	$(CC) -L ../.. -o main *.o $(LIBS)
	rm *.o

