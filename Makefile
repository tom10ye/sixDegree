# A simple makefile for CSE 100 PA4

CC=g++
CXXFLAGS=-std=c++11
LDFLAGS=

# if passed "type=opt" at command-line, compile with "-O3" flag (otherwise use "-g" for debugging)

ifeq ($(type),opt)
    CPPFLAGS += -O3
    LDFLAGS += -O3
else
    CPPFLAGS += -g
    LDFLAGS += -g
endif

all: pathfinder actorconnections extension


ActorGraph.o:  Movie.h ActorNode.h Edge.h ActorGraph.h #UnionFind.hpp

ActorNode.o: ActorNode.h Movie.h

Movie.o: Movie.h Edge.h

Edge.o: Edge.h

UnionFind.o: ActorNode.h Movie.h

util.o: util.h

FBGraph.o: FBNode.h FBGraph.h

FBNode.o: FBNode.h
# include what ever source code *.h files pathfinder relies on (these are merely the ones that were used in the solution)


pathfinder: ActorGraph.o ActorNode.o Edge.o Movie.o util.o

actorconnections: ActorGraph.o ActorNode.o Edge.o Movie.o UnionFind.o util.o

extension: FBGraph.o FBNode.o
# include what ever source code *.h files ActorGraph relies on (these are merely the ones that were used in the solution)

# Note: you do not have to include a *.cpp file if it aleady has a paired *.h file that is already included with class/method headers



clean:
	rm -f pathfinder *.o core*

