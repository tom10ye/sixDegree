#include "ActorNode.h"

ActorNode::ActorNode(string n, int d){
	name = n;
	distance = d;
	done = false;
	prev = NULL;
	prevMovie = NULL;
}

ActorNode::~ActorNode(){

}

void ActorNode::resetNode(){
	distance = INT_MAX;
	prev = NULL;
	prevMovie = NULL;
	done = false;
}