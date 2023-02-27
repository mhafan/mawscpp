#
CXXFLAGS=-std=c++11 -fPIC

all: main.o
	g++ -std=c++11 main.o libmawsgo.a -o main -lcurl

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

clean:
	rm -f main *.o dep *.a
	
