#include "FBGraph.h"
#include <queue>
#include <algorithm>
using namespace std;

FBGraph::FBGraph(void) {}

FBGraph::~FBGraph(){
    //delete all ActorNode and its own edges
    auto it = People.begin();
    auto ed = People.end();
    while(it!=ed){
        delete(it->second);
        it++;
    }

}

bool FBGraph::loadFromFile(const char* in_filename) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
  
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        if (have_header) {
            // skip the header
            have_header = false;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, ' ' )) break;

            record.push_back( next );
        }
    
        if (record.size() != 2) {
            // we should have exactly 3 columns
            continue;
        }

        int ID1 = atoi(record[0].c_str());
        int ID2 = atoi(record[1].c_str());
    
        // we have an actor/movie relationship, now what?
        //insert the information into the graph
        insertInfo(ID1,ID2);
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}

void FBGraph::insertInfo(int ID1, int ID2){
    
    if(People.count(ID1) == 0){
        FBNode* node = new FBNode(ID1, -1);
        People[ID1] = node;
    }

    if(People.count(ID2) == 0){
        FBNode* node = new FBNode(ID2, -1);
        People[ID2] = node;
    }

    //Then take out the entity through their index
    FBNode* Node1 = People[ID1];
    FBNode* Node2 = People[ID2];

    Node1->adj[ID2] = Node2;
    //Node2->adj[ID1] = Node1;
}

void FBGraph::BFS(int ID){
    //initialize the graph
    resetGraph();

    //First take out the startActorNode according to his name
    //set the distance to be0
    FBNode* startNode = People[ID];
    startNode->distance = 0;

    //Initialize the queue for BFS
    queue<FBNode*> Q;

    //push the startActorNode into queue
    Q.push(startNode);

    while(!Q.empty()){
        //pop out from queue
        FBNode* currNode = Q.front();
        Q.pop();

        //traverse through all the edges of currNode
        auto it = currNode->adj.begin();
        auto ed = currNode->adj.end();
        while(it!=ed){
            
            FBNode* neiNode = it->second;

            //if neighbourNode has never be visited before
            if(neiNode->distance == INT_MAX){
                neiNode->prev = currNode;
                neiNode->distance = currNode->distance + 1;

                //after setting all the parameters of the node, check if the node is the end_actor, then break the nested loop
                // if(neiNode->name == end_actor_name) goto breakloop;

                //push node to the queue
                Q.push(neiNode);
            }
            it++;
        }
    }
    // breakloop: return;
}

void FBGraph::recommendNewFriend(int ID, ofstream& out){
    // FBNode startNode = People[ID];
    BFS(ID);
    cout<<"BFS completed!"<<endl;

    map<int, vector<int>> recMap;
    for(auto it = People.begin(); it!= People.end(); it++){
        FBNode* currNode = it->second;
        if(recMap.count(currNode->distance) == 0){
            vector<int> vec;
            recMap[currNode->distance] = vec;
        }
        recMap[currNode->distance].push_back(currNode->ID);
    }

    vector<int> recVector = recMap[2];
    out<<"Recommending friend for customer "<<ID<<": friend(Number of common friends (decreasing order))"<<endl;
    map<int,int> outMap;
    for(int i =0; i<recVector.size();i++){
        //out<<recVector[i];
        int friendID = recVector[i];
        int comNum = findCommonNum(ID,friendID);
        outMap[friendID] = comNum;
        //out<<"("<<comNum<<")"<<endl;
    }
    vector<pair<int,int>> t_vec;
    sortMapbyValue(outMap, t_vec);

    for(auto it = t_vec.begin(); it!= t_vec.end();it++){
        out<<it->first<<"("<<it->second<<")"<<endl;
    }



    out<<"Maybe you also know these people?"<<endl;
    for(int dist = 3; dist<7; dist++){
        out<<"Some one who is "<<dist<<" people away from you:"<<endl;
        vector<int> mayVector = recMap[dist];
        for(int i =0; i<mayVector.size();i++){
            out<<mayVector[i]<<" ";
        }
        out<<endl;
    }


}

void FBGraph::sortMapbyValue(map<int,int> &t_map, vector< pair<int,int> > &t_vec)  {  
    for(map<int,int>::iterator iter = t_map.begin();iter != t_map.end(); iter ++)  
        {  
            t_vec.push_back(make_pair(iter->first,iter->second));  
        }  
  
        sort(t_vec.begin(),t_vec.end(),cmp);
}

bool FBGraph::cmp(const pair<int,int> &x,const pair<int,int> &y){  
    return x.second > y.second;  
}  

int FBGraph::findCommonNum(int ID1, int ID2){
    FBNode* Node1 = People[ID1];
    FBNode* Node2 = People[ID2];

    auto it1 = Node1->adj.begin();
    auto it2 = Node2->adj.begin();

    int total = 0;

    while(it1 != Node1->adj.end() && it2 != Node2->adj.end()){
        // cout<<total<<endl;
        if(it1->first == it2->first){
            total++;
            it1++;
            it2++;
        }else if(it1->first < it2->first){
            it1++;
        }else if(it1->first > it2->first){
            it2++;
        }
    }

    return total;
}

void FBGraph::resetGraph(){
    for(auto it = People.begin(); it != People.end(); it++){
        it->second->resetNode();
    }
}