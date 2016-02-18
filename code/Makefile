CXXFLAGS = -std=c++11 -O3 -Wall -Wextra -pedantic -DNDEBUG

default:
	g++ -I. $(CXXFLAGS) -c *.cpp RJObject/*.cpp RJObject/ConditionalPriors/*.cpp
	ar rcs libdnest4.a *.o
	rm -f *.o
	make nolib -C Examples/SpikeSlab
	make nolib -C Examples/StraightLine
	make nolib -C Examples/RJObject_SineWaves
	make nolib -C Examples/RJObject_GalaxyField
	make nolib -C Examples/ABC

noexamples:
	g++ $(CXXFLAGS) -c *.cpp
	ar rcs libdnest4.a *.o
	rm -f *.o

