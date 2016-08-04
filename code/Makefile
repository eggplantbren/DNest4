CXX = g++
export CXX
CXXFLAGS = -std=c++11 -O3 -Wall -Wextra -pedantic -DNDEBUG

default:
	$(CXX) -I. $(CXXFLAGS) -c *.cpp Distributions/*.cpp RJObject/ConditionalPriors/*.cpp
	ar rcs libdnest4.a *.o
	rm -f *.o
	make nolib -C Examples/SpikeSlab
	make nolib -C Examples/StraightLine
	make nolib -C Examples/RJObject_1DMixture
	make nolib -C Examples/RJObject_SineWaves
	make nolib -C Examples/RJObject_GalaxyField
	make nolib -C Examples/ABC
	make nolib -C Examples/Rosenbrock

noexamples:
	$(CXX) $(CXXFLAGS) -c *.cpp
	ar rcs libdnest4.a *.o
	rm -f *.o

windows:
	x86_64-w64-mingw32-g++ -DNO_THREADS -I. $(CXXFLAGS) -c Level.cpp Options.cpp Utils.cpp CommandLineOptions.cpp LikelihoodType.cpp RNG.cpp Distributions/*.cpp RJObject/ConditionalPriors/*.cpp
	x86_64-w64-mingw32-ar rcs dnest4.lib *.o
	rm -f *.o

