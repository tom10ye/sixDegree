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
	map<int,map<string, Movie*>> MovieTable;
	//map<string, Movie*> oByYear_Movies;
	set<int> ordered_years;

	UnionFind();
	~UnionFind();

	ActorNode* find(ActorNode* n);

	void merge(ActorNode* n1, ActorNode* n2);

	int findBeginYear(string start_actor_name,string end_actor_name);

	bool loadFromFile(const char* in_filename);

	void insertInfo(string actor_name, string movie_title, int movie_year);

	void reset();

	unordered_map<string, int> findRes(unordered_set<string> uniquePairs);

	bool isConnected(string start_actor_name, string end_actor_name);

	void mergeAll(int currentYear);
};



#endif