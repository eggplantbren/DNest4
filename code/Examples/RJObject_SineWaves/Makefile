CXXFLAGS = -std=c++11 -O3 -Wall -Wextra -pedantic -DNDEBUG
LIBS = -ldnest4 -lpthread

default:
	make noexamples -C ../..
	g++ -I../.. $(CXXFLAGS) -c *.cpp
	g++ -L../.. -o main *.o $(LIBS)
	rm *.o

nolib:
	g++ -I../.. $(CXXFLAGS) -c *.cpp
	g++ -L../.. -o main *.o $(LIBS)
	rm *.o

