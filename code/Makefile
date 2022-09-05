CXXFLAGS = -std=c++17 -O3 -march=native -Wall -Wextra -pedantic -DNDEBUG
CXXFLAGS += -Wno-inconsistent-missing-override

# ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
#     detected_OS := Windows
# else
#     detected_OS := $(shell uname)  # same as "uname -s"
# endif

LIBTOOL = ar rcs
ifeq ($(shell uname), Darwin)
	LIBTOOL = libtool -static -o
endif

SRCS = $(wildcard *.cpp) \
       $(wildcard Distributions/*.cpp) \
       $(wildcard RJObject/ConditionalPriors/*.cpp)

OBJS = $(patsubst %.cpp,%.o,$(SRCS))

all: examples $(OBJS) libdnest4.a
noexamples: $(OBJS) libdnest4.a

libdnest4.a: $(OBJS)
	@echo Creating $@ library
	@echo $(LIBTOOL)
	@$(LIBTOOL) $@ $^

examples: $(OBJS) libdnest4.a
	make nolib -C Examples/SpikeSlab
	make nolib -C Examples/StraightLine
	make nolib -C Examples/RJObject_1DMixture
	make nolib -C Examples/RJObject_SineWaves
	make nolib -C Examples/RJObject_GalaxyField
	make nolib -C Examples/ABC
	make nolib -C Examples/Rosenbrock
	make nolib -C Examples/Rosenbrock2
	make nolib -C Examples/LennardJones
	make nolib -C Examples/Optimizer


windows:
	x86_64-w64-mingw32-g++-posix -I. -std=c++11 -O3 -Wall -Wextra -pedantic -DNDEBUG -c $(SRCS)
	x86_64-w64-mingw32-ar rcs dnest4.lib *.o
	rm -f *.o

clean:
	rm -f libdnest4.a
	rm -f *.o Distributions/*.o
