#ifndef FBNODE_H
#define FBNODE_H

#include <iostream>
#include <map>
#include <string>
#include <limits.h>
using namespace std;

class FBNode{
public:
	int ID;
	map<int, FBNode*> adj;
	int distance;

	FBNode* prev;

	FBNode(int I, int d);

	~FBNode();

	void resetNode();
};




// class ActorNodeComp{
// public:
// 	bool operator()(ActorNode*& A, ActorNode*& B) const {
//         return A->distance > B->distance;
//     }
// };

#endif