CXXFLAGS = -std=c++11 -O3 -Wall -Wextra -pedantic

default:
	g++ $(CXXFLAGS) -c *.cpp
	ar rcs libdnest4.a *.o
	rm -f *.o
	make nolib -C Examples/SpikeSlab
	make nolib -C Examples/StraightLine

noexample:
	g++ $(CXXFLAGS) -c *.cpp
	ar rcs libdnest4.a *.o
	rm -f *.o

