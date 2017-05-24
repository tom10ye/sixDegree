/*
 * ActorGraph.h
 * Author: Cheng Qian
 * Date:   5-21-2017
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <unordered_map>
#include "ActorNode.h"
#include "Edge.h"
#include "Movie.h"

// Maybe include some data structures here

using namespace std;

class ActorGraph {
protected:
  
    // Maybe add class data structure(s) here
    unordered_map<string, ActorNode*> Actors;
    unordered_map<string, Movie*> Movies;

public:
    ActorGraph(void);

    // Maybe add some more methods here
  
    ~ActorGraph();
    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2015 - movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);

    void printSta();

    void insertInfo(string actor_name, string movie_title, int movie_year);

    void insertEdgeForStart(ActorNode* start, ActorNode* end, Movie* m);

    void BFS(string start_actor_name, string end_actor_name);

    void resetGraph();

    void backTrackOut(string start_actor_name, string end_actor_name, ofstream& out);
  
};


#endif // ACTORGRAPH_H