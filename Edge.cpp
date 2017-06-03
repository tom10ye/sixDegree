#include "Edge.h"

Edge::Edge(ActorNode* s, ActorNode* e, Movie* m){
	start = s;
	end = e;
	movie = m;
	weight = 0;
}

Edge::~Edge(){}