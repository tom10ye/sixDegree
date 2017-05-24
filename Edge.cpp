#include "Edge.h"

Edge::Edge(ActorNode* s, ActorNode* e, Movie* m){
	start = s;
	end = e;
	movie = m;
}

Edge::~Edge(){}