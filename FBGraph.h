
#ifndef FBGRAPH_H
#define FBGRAPH_H

#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <stdlib.h> 

#include "FBNode.h"


using namespace std;

class FBGraph {
protected:
  
    // Maybe add class data structure(s) here
    unordered_map<int, FBNode*> People;

public:
    FBGraph(void);

    // Maybe add some more methods here
  
    ~FBGraph();
    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2015 - movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* in_filename);

    // void printSta();

    void insertInfo(int ID1, int ID2);

    // void insertInfoConnection(string actor_name, string movie_title, int movie_year);

    // void insertEdgeForStart(ActorNode* start, ActorNode* end, Movie* m);

    void BFS(int ID);
    void recommendNewFriend(int ID, ofstream& out);
    int findCommonNum(int ID1, int ID2);

    void sortMapbyValue(map<int,int> &t_map, vector< pair<int,int> > &t_vec)  ;

    // void Dijkstra(string start_actor_name,string end_actor_name);

    void resetGraph();

    // void backTrackOut(string start_actor_name, string end_actor_name, ofstream& out);
  
    // int BFSFind(string start_actor_name, string end_actor_name);

    // bool isConnected(string start_actor_name, string end_actor_name);

    // unordered_map<string, int> findRes(unordered_set<string> uniquePairs);
    static bool cmp(const pair<int,int> &x,const pair<int,int> &y);
};

    


#endif // ACTORGRAPH_H