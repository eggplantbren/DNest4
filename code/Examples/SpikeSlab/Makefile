default:
	make noexample -C ../..
	g++ -I../.. -std=c++11 -O3 -Wall -Wextra -pedantic -c *.cpp
	g++ -L../.. -o main *.o -ldnest4 -lpthread
	rm *.o

nolib:
	g++ -I../.. -std=c++11 -O3 -Wall -Wextra -pedantic -c *.cpp
	g++ -L../.. -o main *.o -ldnest4 -lpthread
	rm *.o

