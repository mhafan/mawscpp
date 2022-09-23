testing:
	g++ -std=c++11 main.cpp -o main -lcurl 

clean:
	rm -f main *.o
	
install:
	cp mawscpp.h ~/local/include