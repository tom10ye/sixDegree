#ifndef UNIONFIND_H
#define UNIONFIND_H

#include "ActorGraph.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

class UnionFind{
public:
	unordered_map<string, ActorNode*> Actors;
	map<string, Movie*> oByYear_Movies;

	UnionFind();
	~UnionFind();

	ActorNode* find(ActorNode* n);

	void merge(ActorNode* n1, ActorNode* n2);

	int findBeginYear(string start_actor_name,string end_actor_name);

	bool loadFromFile(const char* in_filename);

	void insertInfo(string actor_name, string movie_title, int movie_year);

	void reset();
};



#endif