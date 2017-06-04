#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include "FBGraph.h"
#include "FBNode.h"


using namespace std;

int main(int argc, char* argv[]){
	
	//1.Initialization
	//Input format:  movie_casts.tsv u/w pairs output
	// char* infile = argv[3];
	char* outfile = argv[3];

	// ifstream in;
	ofstream out;

	// in.open(infile, ios::in);
	out.open(outfile, ios::out);

	

	FBGraph* G = new FBGraph();
	cout<<"Initialization Completed!"<<endl;


	//2.Load data and print statistics
	G->loadFromFile(argv[1]);

	cout<<"Load Completed!"<<endl;
	//G->printSta();
	
	int num =atoi(argv[2]);
	
	G->recommendNewFriend(num,out);
	delete(G);
	// in.close();
	// out.close();
	return 0;
}