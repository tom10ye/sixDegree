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
#include <unordered_set>
#include <unordered_map>

#include "ActorGraph.h"
#include "ActorNode.h"
#include "Edge.h"
#include "Movie.h"
#include "UnionFind.h"
#include "util.h"

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
		G->loadFromFileConnection(argv[1]);
		cout<<"Load Completed!"<<endl;
	}else{
        UF->loadFromFile(argv[1]);
        cout<<"Load Completed!"<<endl;
	}
	
	//G->printSta();
	

	//3.Iteratively find the shortest path of each pairs
	out << "Actor1\t" << "Actor2\t" << "Year" << endl;
	bool have_header = true;

	//Initialize the data structure we gonna use
	vector<pair<string,string>> outVector;
	unordered_set<string> uniquePairs;
	unordered_map<string, int> uniqueRes;

	//int iter = 0;
	Timer T;
	long time_duration;
	T.begin_timer();

	//load all the pairs in to system
	while(in){
		//cout<<"iter:"<<++iter<<endl;
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

        if(start_actor_name == end_actor_name){
        	cout<<"Please input two different names!"<<endl;
        	return -1;
        }

        pair<string, string> currPair(start_actor_name, end_actor_name);
        outVector.push_back(currPair);

        string currString = start_actor_name + "\t" + end_actor_name;
        uniquePairs.insert(currString);

        // int beginYear;
        // if(useBFS == true){
        // 	G->resetGraph();
        // 	beginYear = G->BFSFind(start_actor_name,end_actor_name);
        // 	G->removeAllEdges();
        	
        // }else{
        // 	UF->reset();
        // 	beginYear = UF->findBeginYear(start_actor_name,end_actor_name);
        // }
        // out << start_actor_name << "\t" << end_actor_name << "\t" << beginYear << endl; 

	}


	if(useBFS == true){
    	uniqueRes = G->findRes(uniquePairs);
    }else{
    	uniqueRes = UF->findRes(uniquePairs);
    	cout<<"finish result!"<<endl;
    }

    for(int i=0; i< outVector.size();i++ ){
    	int beginYear = uniqueRes[outVector[i].first + outVector[i].second];
    	string start_actor_name = outVector[i].first;
    	string end_actor_name = outVector[i].second;
    	out << start_actor_name << "\t" << end_actor_name << "\t" << beginYear << endl; 
    }

	time_duration = T.end_timer();
	cout<<"time:"<<time_duration<<"(nanosecond)  =  "<<time_duration/double(1000000000)<<"(second)"<<endl;
	

	delete(G);
	delete(UF);
	in.close();
	out.close();
	return 0;
}