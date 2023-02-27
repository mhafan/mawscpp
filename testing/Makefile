#
CXXFLAGS=-std=c++11 -fPIC

all: main.o
	g++ -std=c++11 -L/usr/local/lib main.o libmawsgo.a -o main -lcurl

main.o: main.cpp
	g++ -std=c++11 -c -I/usr/local/include main.cpp

clean:
	rm -f main *.o dep *.a
	
