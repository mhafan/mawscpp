#
LIB_FILES=basics.cpp eman.cpp
LIB_OBJS=basics.o eman.o

#
DOCKER_HEADERS=/usr/local/include
DOCKER_LIBS=/usr/local/lib

#
CXXFLAGS=-std=c++11 -fPIC

#
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
	
clean:
	rm -f main *.o dep *.a
	
install:
	cp *.hpp $(DOCKER_HEADERS)
	cp *.a $(DOCKER_LIBS)