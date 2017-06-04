#include "FBNode.h"

FBNode::FBNode(int I, int d){
	ID = I;
	distance = d;
	prev = NULL;
}

FBNode::~FBNode(){

}

void FBNode::resetNode(){
	distance = INT_MAX;
	prev = NULL;
}
