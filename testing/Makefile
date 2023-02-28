#
CXXFLAGS=-std=c++11 -fPIC -I ../ -I/usr/local/include

all:
	g++ $(CXXFLAGS) main.cpp -o main -lcurl

clean:
	rm -f main *.o dep *.a
	
docker:
	docker build -t mawsgo:cpp . 
	