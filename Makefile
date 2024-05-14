CXX = g++
CFLAGS = -w -g

all: main

main: main.o graph.o
	$(CXX) $(CFLAGS) -o main main.o graph.o

main.o: main.cpp
	$(CXX) $(CFLAGS) -c main.cpp

graph.o: graph.cpp
	$(CXX) $(CFLAGS) -c graph.cpp

clean:
	rm -f *.o main out*