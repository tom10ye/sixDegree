#include "UnionFind.h"
#include <unordered_set>

using namespace std;

UnionFind::UnionFind(){};

UnionFind::~UnionFind(){
	auto it = Actors.begin();
    auto ed = Actors.end();
    while(it!=ed){
        ActorNode* currNode = it->second;
        delete(currNode);
        it++;
    }

    for(auto it=MovieTable.begin(); it!=MovieTable.end(); it++){
        map<string, Movie*> movMap = it->second;
        for(auto it2=movMap.begin(); it2!=movMap.end(); it2++){
            delete(it2->second);
        }
    }
    //delete all movies
    // auto it_mov = oByYear_Movies.begin();
    // auto ed_mov = oByYear_Movies.end();
    // while(it_mov!=ed_mov){
    //     delete(it_mov->second);
    //     it_mov++;
    // }
}

ActorNode* UnionFind::find(ActorNode* n){
    // cout<<"enter find"<<endl;
    
    // if(curr != NULL) cout<< "n is not null"<<endl;
    // if(curr->prev!= NULL) cout<< "n'prev is not null"<<endl;
	if(n->prev == NULL) return n;
    ActorNode* curr = n;
	unordered_set<ActorNode*> pathNodeSet;
	while(curr->prev != NULL){
		pathNodeSet.insert(curr);
		curr = curr->prev;
        //cout<<curr->name<<endl;

	}
	auto it = pathNodeSet.begin();
	auto ed = pathNodeSet.end();
	while(it!=ed){
		(*it)->prev = curr;
		it++;
	}
	return curr;
}

void UnionFind::merge(ActorNode* n1, ActorNode* n2){
	//find the root
	ActorNode* r1 = find(n1);
	ActorNode* r2 = find(n2);

	if(r1==r2) return;
	r2->prev = r1;
}

// int UnionFind::findBeginYear(string start_actor_name,string end_actor_name){
// 	auto it = oByYear_Movies.begin();
// 	auto ed = oByYear_Movies.end();
// 	int originYear = it->second->year;
// 	while(it!=ed){
// 		Movie* m = it->second;
// 		if(m->year != originYear){
// 			//cout<<"check it"<<endl;
// 			if(find(Actors[start_actor_name]) == find(Actors[end_actor_name])){
// 			    return originYear;
// 			}
// 		}
// 		auto it_cast = m->casts.begin();
// 		auto ed_cast = m->casts.end();
// 		while(it_cast!=ed_cast){
// 			merge((m->casts.begin())->second, it_cast->second);
// 			it_cast++;
// 		}
		
// 		originYear = m->year;
// 		it++;
// 	}

// 	return 9999;
// }

bool UnionFind::loadFromFile(const char* in_filename) {
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

void UnionFind::insertInfo(string actor_name, string movie_title, int movie_year){
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
    if(MovieTable[movie_year].count(movie_title) == 0){
        Movie* mov = new Movie(movie_title,movie_year);
        MovieTable[movie_year][movie_title] = mov;
    }


    ActorNode* currActorNode = Actors[actor_name];
    Movie* mov = MovieTable[movie_year][movie_title];
    mov->casts[actor_name] =currActorNode;
    //
    ordered_years.insert(movie_year);
    //cout<<"inserting info compl"<<endl;
}

void UnionFind::reset(){
	auto it = Actors.begin();
    auto ed = Actors.end();
    while(it != ed){
        it->second->resetNode();
        it++;
    }
}

void UnionFind::mergeAll(int currentYear){
    auto movMap = MovieTable[currentYear];

    auto it = movMap.begin();
    auto ed = movMap.end();
    while(it != ed){
        Movie* mov = it->second;
        for(auto itcast = mov->casts.begin(); itcast != mov->casts.end(); itcast++){
            ActorNode* endActorNode = itcast->second;
            merge((mov->casts.begin())->second, endActorNode);
        }
        it++;
    }
}

bool UnionFind::isConnected(string start_actor_name, string end_actor_name){
    ActorNode* start = Actors[start_actor_name];
    ActorNode* end = Actors[end_actor_name];
    
    ActorNode* root1 = find(start);
    ActorNode* root2 = find(end);
    if(root1 == root2){
        return true;
    }else return false;
}

unordered_map<string, int> UnionFind::findRes(unordered_set<string> uniquePairs){
    unordered_map<string, int> resMap;
    auto it = ordered_years.begin();
    auto ed = ordered_years.end();
    while(it!=ed){
        int currentYear = *it;
        mergeAll(currentYear);
        //cout<<"mergeAll completed"<<endl;
        //cout<<UpperLimitYear<<endl;
        vector<string> toBeErase;
        for(auto it = uniquePairs.begin(); it != uniquePairs.end(); it++){
            string s = *it;

            istringstream ss(s);
            vector <string> record;

            while(ss){
                string next;
                if (!getline( ss, next, '\t' )) break;
                record.push_back( next );
            }
            //cout<<"get record Completed"<<endl;
            // if (record.size() != 2) {
            //     continue;
            // }

            string start_actor_name = record[0];
            string end_actor_name = record[1];
            //cout<<"get record"<<endl;
            // string start_actor_name = it->first;
            // string end_actor_name = it->second;
            if(isConnected(start_actor_name, end_actor_name) == true){
                //cout<<"enter the if"<<endl;
                resMap[start_actor_name + end_actor_name] = currentYear;
                toBeErase.push_back(*it);
            }
            //cout<<"What?"<<endl;
        }

        for(int i=0;i<toBeErase.size();i++){
            uniquePairs.erase(toBeErase[i]);
        }

        if(uniquePairs.empty() == true) break;
        
        //cout<<"OK!"<<endl;
        it++;
    }
    return resMap;
}