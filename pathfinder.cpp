#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include "ActorGraph.h"
#include "ActorNode.h"
#include "Edge.h"
#include "Movie.h"
#include "util.h"

using namespace std;

int main(int argc, char* argv[]){
	
	//1.Initialization
	//Input format:  movie_casts.tsv u/w pairs output
	char* infile = argv[3];
	char* outfile = argv[4];

	ifstream in;
	ofstream out;

	in.open(infile, ios::in);
	out.open(outfile, ios::out);

	//u means unweithed, if input is u then the statement is false, we do not use weighted edges
	bool use_weighted_edges;
	if(*argv[2] == 'u'){
		use_weighted_edges = false;
	}else if(*argv[2] == 'w'){
		use_weighted_edges = true;
	}else{
		cout<<"please input valid weighted/unweighted flag"<<endl;
		return 0;
	}

	ActorGraph* G = new ActorGraph();
	cout<<"Initialization Completed!"<<endl;


	//2.Load data and print statistics
	G->loadFromFile(argv[1],use_weighted_edges);
	cout<<"Load Completed!"<<endl;
	//G->printSta();
	

	//3.Iteratively find the shortest path of each pairs
	out << "(actor)--[movie#@year]-->(actor)--..." << endl;
	bool have_header = true;

	Timer T;
	long time_duration;
	T.begin_timer();
	while(in){
		//cout<<"interative"<<endl;
		string s;
		if (!getline(in, s)) break;
		//cout<<"getline Completed"<<endl;
        if (have_header) {
            // skip the header
            have_header = false;
            continue;
        }

        istringstream ss(s);
        vector <string> record;

        while(ss){
        	string next;
        	if (!getline( ss, next, '\t' )) break;
            record.push_back( next );
        }
        //cout<<"get record Completed"<<endl;
        if (record.size() != 2) {
            continue;
        }

        string start_actor_name = record[0];
        string end_actor_name = record[1];
        if(G->nameExist(start_actor_name, end_actor_name) == false){
        	out << "name not exist in the graph" <<endl;
        	continue;
        }

        //G->resetGraph();

        if(use_weighted_edges == false){
        	//cout<<"Begin BFS"<<endl;
        	/*
			Here we first call BFS function to set up all the distance/prev/prevMovie of the nodes in the path
			And at last call backtrack function to print out the path
        	*/
        	G->BFS(start_actor_name,end_actor_name);
        	//cout<<"BFS Completed!"<<endl;
        }else{
        	G->resetGraph();
        	G->Dijkstra(start_actor_name,end_actor_name);
        }

        G->backTrackOut(start_actor_name,end_actor_name,out);
        //cout<<"backTrackOut Completed!"<<endl;

	}

	time_duration = T.end_timer();
	cout<<"time:"<<time_duration<<"(nanosecond)  =  "<<time_duration/double(1000000000)<<"(second)"<<endl;

	delete(G);
	in.close();
	out.close();
	return 0;
}