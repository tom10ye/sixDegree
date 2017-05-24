#ifndef EDGE_H
#define EDGE_H

//#include "Movie.h"

using namespace std;

class ActorNode;
class Movie;
class Edge{
public:
	Movie* movie;
	ActorNode* start;
	ActorNode* end;

	Edge(ActorNode* s, ActorNode* e, Movie* m);

	~Edge();
};

#endif