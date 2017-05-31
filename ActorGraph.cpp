/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */
 
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <limits.h>
#include "ActorGraph.h"

using namespace std;

ActorGraph::ActorGraph(void) {}

ActorGraph::~ActorGraph(){
    //delete all ActorNode and its own edges
    auto it = Actors.begin();
    auto ed = Actors.end();
    while(it!=ed){
        ActorNode* currNode = it->second;
        auto it_edge = currNode->edges.begin();
        auto ed_edge = currNode->edges.end();
        while(it_edge!=ed_edge){
            delete(it_edge->second);
            it_edge++;
        }
        delete(currNode);
        it++;
    }

    //delete all movies
    auto it_mov = Movies.begin();
    auto ed_mov = Movies.end();
    while(it_mov!=ed_mov){
        delete(it_mov->second);
        it_mov++;
    }
}

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
  
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);
    
        // we have an actor/movie relationship, now what?
        //insert the information into the graph
        insertInfo(actor_name, movie_title, movie_year);
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}

void ActorGraph::printSta(){
    auto it = Actors.begin();
    auto ed = Actors.end();
    int totalEdges;
    while(it!= ed){
        totalEdges = totalEdges + it->second->edges.size();
        it++;
    }
    cout<<"Statistics:\nActors: "<<Actors.size()<<"\nMovies: "<<Movies.size()<<"\nEdges: "<<totalEdges<<endl;
}

void ActorGraph::insertInfo(string actor_name, string movie_title, int movie_year){
    //cout<<"begin inserting info"<<endl;
    //first update in the database of ActorGraph
    //if the actor or movie has never been inserted before, initialize the entity and store the ptr into the database
    
    //note that there will not be two actors have the same name
    //so we use name to be the unique identifier of actor
    if(Actors.count(actor_name) == 0){
        ActorNode* node = new ActorNode(actor_name, -1);
        Actors[actor_name] = node;
    }

    //Note that multiple movies made in different years can have the same name
    //So we use title+year to be the unique id of movie
    string movieID = movie_title + to_string(movie_year);
    if(Movies.count(movieID) == 0){
        Movie* mov = new Movie(movie_title,movie_year);
        Movies[movieID] = mov;
    }

    //Then take out the entity through their index
    ActorNode* currActorNode = Actors[actor_name];
    Movie* mov = Movies[movieID];

    /*
    Finally we need to insert edges between nodes according to this information
    The main idea is centered at the movie in the info, and make connections between current actor
    with all the actors in the cast list of the movie.
    The graph is undirected, but we can see it as two directed edges retrospectively, so we need to add both edges which take both nodes as start node, seperatively;
    Otherwise, if we just initialize one edge, then the start point and end point will be mixed
    we cannot directly use .end to indicate the next node when bfs
    */
    auto it = mov->casts.begin();
    auto ed = mov->casts.end();
    while(it != ed){
        ActorNode* endActorNode = it->second;
        insertEdgeForStart(currActorNode, endActorNode, mov);
        insertEdgeForStart(endActorNode, currActorNode, mov);
        it++;
    }

    //After all the connections have been made, add current actor on the cast list. To avoid self cycle.
    mov->casts[actor_name] =currActorNode;

    //
    ordered_years.insert(movie_year);
    //cout<<"inserting info compl"<<endl;
}

//Need to do:
//the index of edges should be identified uniquely movietitle+endname
//the title of film is not unique, title+year
//finishi insertEdge

void ActorGraph::insertEdgeForStart(ActorNode* start, ActorNode* end, Movie* m){
    // if(start->edges.count(end->name) == 0){
    //     Edge* edge = new Edge(start, end, m);
    //     start->edges[end->name] = edge;
    // }

    // Edge* edge = start->edges[end->name];
    //need to add something?

    //should all be unique

    //Note that multiple movies made in different years can have the same name
    //such as A and B were both casted in movie M(1994), M(2000), M(2004), which are three different edges
    //So we use title+year+end to be the unique id of edge
    string edgeID = m->title + to_string(m->year) + end->name;

    //if this edge has been added before then return, however this case will not happen, think about it
    if(start->edges.count(edgeID) != 0) return;
    Edge* edge = new Edge(start, end, m);
    start->edges[edgeID] = edge;
}

void ActorGraph::BFS(string start_actor_name, string end_actor_name){
    //initialize the graph
    resetGraph();

    //First take out the startActorNode according to his name
    //set the distance to be0
    ActorNode* startNode = Actors[start_actor_name];
    startNode->distance = 0;

    //Initialize the queue for BFS
    queue<ActorNode*> Q;

    //push the startActorNode into queue
    Q.push(startNode);

    while(!Q.empty()){
        //pop out from queue
        ActorNode* currNode = Q.front();
        Q.pop();

        //traverse through all the edges of currNode
        auto it = currNode->edges.begin();
        auto ed = currNode->edges.end();
        while(it!=ed){
            Edge* currEdge = it->second;
            ActorNode* neiNode = currEdge->end;

            //if neighbourNode has never be visited before
            if(neiNode->distance == INT_MAX){
                neiNode->prev = currNode;
                neiNode->prevMovie = currEdge->movie;
                neiNode->distance = currNode->distance + 1;

                //after setting all the parameters of the node, check if the node is the end_actor, then break the nested loop
                if(neiNode->name == end_actor_name) goto breakloop;

                //push node to the queue
                Q.push(neiNode);
            }

            it++;
        }
    }
    breakloop: return;
}

void ActorGraph::resetGraph(){
    auto it = Actors.begin();
    auto ed = Actors.end();
    while(it != ed){
        it->second->resetNode();
        it++;
    }
}

void ActorGraph::Dijkstra(string start_actor_name, string end_actor_name){
    //initialize the graph
    resetGraph();

    //First take out the startActorNode according to his name
    //set the distance to be0
    ActorNode* startNode = Actors[start_actor_name];
    startNode->distance = 0;

    //Initialize PQ for Dijkstra
    priority_queue<pair<ActorNode*, int>, vector<pair<ActorNode*, int>>,ActorNodeComp> PQ;

    //push the startActorNode into queue
    pair<ActorNode*, int> startPair(startNode, 0);
    PQ.push(startPair);

    while(!PQ.empty()){
        //pop out from PQ
        pair<ActorNode*, int> currPair = PQ.top();
        ActorNode* currNode = currPair.first;
        PQ.pop();
        if(currNode->done == false){

            currNode->done = true;

            //traverse through all the edges of currNode
            auto it = currNode->edges.begin();
            auto ed = currNode->edges.end();
            while(it!=ed){
                Edge* currEdge = it->second;
                ActorNode* neiNode = currEdge->end;

                int edgeWeight = 2015-(currEdge->movie->year) +1;
                int newDist = currNode->distance + edgeWeight;

                //if neighbourNode's distance is not minimum
                if(newDist < neiNode->distance ){
                    neiNode->prev = currNode;
                    neiNode->prevMovie = currEdge->movie;
                    neiNode->distance = newDist;

                    //push newPair to the PQ
                    pair<ActorNode*, int> nextPair(neiNode, newDist);
                    PQ.push(nextPair);
                }

                it++;
            }
        }
    }

}

void ActorGraph::backTrackOut(string start_actor_name, string end_actor_name, ofstream& out){
    
    /*
    we have set up all the prev ptr, so we only need to backtrack from the end node, all the way back to the start node
    use a stack to  record all the node on the path, and output the node sequentially
    */

    stack<ActorNode*> stack;
    ActorNode* currNode = Actors[end_actor_name];
    int correctDist = currNode->distance;

    //Iteratively push the node on the path into the stack, until the second last node on the path
    while(true){
        //First, check if the current node (end node) is the start node
        string currName = currNode->name;
        if(currName == start_actor_name) break;
        stack.push(currNode);
        currNode = currNode->prev;
    }

    //check if the stack size matach the distance of the path ,use for bfs
    // if(stack.size() != correctDist){
    //     out<<"Wrong size of stack when backtrack"<<endl;
    //     return;
    // }

    //after the iterations, here,current node is the start_actor's node,first print its name
    out<< "(" << currNode->name << ")";

    //while the stack is not empty, pop the nodes out and print its previous movie first and its name secondly
    while(!stack.empty()){
        ActorNode* neiNode = stack.top();
        stack.pop();
        out<< "--[" << neiNode->prevMovie->title << "#@" << to_string(neiNode->prevMovie->year);
        out<<"]-->(" << neiNode->name << ")";
    }

    out<<endl;
}

int ActorGraph::BFSFind(string start_actor_name, string end_actor_name){
    auto it = ordered_years.begin();
    auto ed = ordered_years.end();
    while(it!=ed){
        int UpperLimitYear = *it;
        //cout<<UpperLimitYear<<endl;
        if(BFSYear(start_actor_name, end_actor_name, UpperLimitYear) == true) return UpperLimitYear;
        it++;
    }
    return 9999;
}

bool ActorGraph::BFSYear(string start_actor_name, string end_actor_name, int UpperLimitYear){
    //initialize the graph
    resetGraph();

    //First take out the startActorNode according to his name
    //set the distance to be 0
    ActorNode* startNode = Actors[start_actor_name];
    startNode->distance = 0;

    //Initialize the queue for BFS
    queue<ActorNode*> Q;

    //push the startActorNode into queue
    Q.push(startNode);

    while(!Q.empty()){
        //pop out from queue
        ActorNode* currNode = Q.front();
        Q.pop();

        //traverse through all the edges of currNode
        auto it = currNode->edges.begin();
        auto ed = currNode->edges.end();
        while(it!=ed){
            Edge* currEdge = it->second;

            //if the current edge is valid when this year, it can used for BFS, or we skip it
            if(currEdge->movie->year <= UpperLimitYear){
                ActorNode* neiNode = currEdge->end;

                //if neighbourNode has never be visited before
                if(neiNode->distance == INT_MAX){
                    neiNode->prev = currNode;
                    neiNode->prevMovie = currEdge->movie;
                    neiNode->distance = currNode->distance + 1;

                    //after setting all the parameters of the node, check if the node is the end_actor, then break the nested loop
                    if(neiNode->name == end_actor_name) goto breakloop;

                    //push node to the queue
                    Q.push(neiNode);
                }
            }

            it++;
        }
    }
    return false;
    breakloop: return true;
}

bool ActorGraph::nameExist(string start_actor_name, string end_actor_name){
    if(Actors.count(start_actor_name) != 0 && Actors.count(end_actor_name) != 0){
        return true;
    }else return false;
}