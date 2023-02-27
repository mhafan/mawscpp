#
LIB_FILES=basics.cpp eman.cpp
LIB_OBJS=basics.o eman.o

#
CXXFLAGS=-std=c++11 -fPIC

OUTFILE=libmawsgo.a

dep: 
	$(CXX) $(CXXFLAGS) -MM $(LIB_FILES) >dep
include dep

all: $(OUTFILE)

$(OUTFILE): $(LIB_OBJS)
	ar ru $@ $^ 
	ranlib $@

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

testing: main.o
	g++ -std=c++11 main.o $(OUTFILE) -o main -lcurl 

clean:
	rm -f main *.o dep *.a

lib: $(LIB_OBJS)
	$(CXX) $(CXXFLAGS)
	
install:
	cp mawscpp.h ~/local/include