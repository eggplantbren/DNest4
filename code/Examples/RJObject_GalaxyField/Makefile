CXXFLAGS = -std=c++11 -O3 -Wall -Wextra -pedantic -DNDEBUG
LIBS = -ldnest4 -lpthread

default:
	make noexamples -C ../..
	$(CXX) -I../.. $(CXXFLAGS) -c *.cpp
	$(CXX) -L../.. -o main *.o $(LIBS)
	rm *.o

nolib:
	$(CXX) -I../.. $(CXXFLAGS) -c *.cpp
	$(CXX) -L../.. -o main *.o $(LIBS)
	rm *.o

