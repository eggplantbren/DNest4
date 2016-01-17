# Put the directory above 'DNest4' into an environment variable
# called DNEST4_PATH for this to work.

CXXFLAGS = -std=c++11 -O3 -Wall -Wextra -pedantic -DNDEBUG
LIBS = -ldnest4 -lpthread -ljulia

default:
	g++ -I$(DNEST4_PATH) $(CXXFLAGS) -c *.cpp
	g++ -L$(JULIA_PATH) -L$(DNEST4_PATH)/DNest4/code -o main *.o $(LIBS)
	rm *.o

