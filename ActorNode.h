#ifndef ACTORNODE_H
#define ACTORNODE_H

#include "Movie.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <limits.h>
using namespace std;

class ActorNode{
public:
	string name;
	unordered_map<string, Edge*> edges;//key is title+year+endname, value is a ptr to that edge
	int distance;

	ActorNode* prev;
	Movie* prevMovie;

	bool done;

	ActorNode(string n, int d);

	~ActorNode();

	void resetNode();
};

class ActorNodeComp{
public:
	bool operator()(ActorNode*& A, ActorNode*& B) const {
        return A->distance > B->distance;
    }
};

#endif