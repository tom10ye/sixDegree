#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <stdio.h>
#include <string.h>
#include <string>
#include <cstring>
#include "ActorGraph.h"
#include "ActorNode.h"
#include "Edge.h"
#include "Movie.h"
#include "UnionFind.h"

using namespace std;

int main(int argc, char* argv[]){
	//Input format:  movie_casts.tsv pairs output bfs/ufind

	char* infile = argv[2];
	char* outfile = argv[3];

	ifstream in;
	ofstream out;

	in.open(infile, ios::in);
	out.open(outfile, ios::out);

	bool useBFS = (strcmp(argv[4],"bfs") == 0);

	
	cout<<"Initialization Completed!"<<endl;


	//2.Load data and print statistics
	ActorGraph* G = new ActorGraph();
	UnionFind* UF =  new UnionFind();
	if(useBFS == true){
		//ActorGraph* G = new ActorGraph();
		G->loadFromFile(argv[1],false);
		cout<<"Load Completed!"<<endl;
	}else{
        UF->loadFromFile(argv[1]);
        cout<<"Load Completed!"<<endl;
	}
	
	//G->printSta();
	

	//3.Iteratively find the shortest path of each pairs
	out << "Actor1\t" << "Actor2\t" << "Year" << endl;
	bool have_header = true;
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

        // if(G->nameExist(start_actor_name, end_actor_name) == false){
        // 	out << start_actor_name << "\t" << end_actor_name << "\t" << "9999";
        // 	continue;
        // }

        //UF name exist function

        if(start_actor_name == end_actor_name){
        	cout<<"Please input two different names!"<<endl;
        	return -1;
        }

        int beginYear;
        if(useBFS == true){
        	G->resetGraph();
        	//cout<<"Begin BFSFind"<<endl;
        	/*
			Here we first call BFS function to set up all the distance/prev/prevMovie of the nodes in the path
			And at last call backtrack function to print out the path
        	*/
        	beginYear = G->BFSFind(start_actor_name,end_actor_name);
        	//cout<<"BFSFind Completed!"<<endl;
        }else{
        	//cout<<"enter unionfind"<<endl;
        	UF->reset();
        	beginYear = UF->findBeginYear(start_actor_name,end_actor_name);
        	
        }
        out << start_actor_name << "\t" << end_actor_name << "\t" << beginYear << endl; 
        //cout<<"backTrackOut Completed!"<<endl;

	}

	
	delete(G);
	
	delete(UF);
	
	
	in.close();
	out.close();
	return 0;
}