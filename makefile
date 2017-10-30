FLAGS = -Wall -std=c++11 -g 

all: BestFirstSearch

BestFirstSearch: bestFirstSearch.o
	g++ $(FLAGS) bestFirstSearch.o -o BestFirstSearch

bestFirstSearch.o: bestFirstSearch.cpp
	g++ $(FLAGS) -c bestFirstSearch.cpp -o bestFirstSearch.o

clean:
	rm -f *.o BestFirstSearch
