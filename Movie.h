#ifndef MOVIE_H
#define MOVIE_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "Edge.h"

using namespace std;

class Movie{
public:
	string title;
	int year;
	unordered_map<string, ActorNode*> casts;
	Movie(string t, int y);

	~Movie();
};

#endif