all: main.cpp
	g++ -std=c++17 main.cpp -o main

clean:
	rm -f main main.o *.out

rsol:
	rm -f solutions/*.solution