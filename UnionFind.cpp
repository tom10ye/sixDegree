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

    //delete all movies
    auto it_mov = oByYear_Movies.begin();
    auto ed_mov = oByYear_Movies.end();
    while(it_mov!=ed_mov){
        delete(it_mov->second);
        it_mov++;
    }
}

ActorNode* UnionFind::find(ActorNode* n){
	if(n->prev == NULL) return n;
	unordered_set<ActorNode*> pathNodeSet;
	while(n->prev != NULL){
		pathNodeSet.insert(n);
		n = n->prev;
	}
	auto it = pathNodeSet.begin();
	auto ed = pathNodeSet.end();
	while(it!=ed){
		(*it)->prev = n;
		it++;
	}
	return n;
}

void UnionFind::merge(ActorNode* n1, ActorNode* n2){
	//find the root
	ActorNode* r1 = find(n1);
	ActorNode* r2 = find(n2);

	if(r1==r2) return;
	r2->prev = r1;
}

int UnionFind::findBeginYear(string start_actor_name,string end_actor_name){
	auto it = oByYear_Movies.begin();
	auto ed = oByYear_Movies.end();
	int originYear = it->second->year;
	while(it!=ed){
		Movie* m = it->second;
		if(m->year != originYear){
			//cout<<"check it"<<endl;
			if(find(Actors[start_actor_name]) == find(Actors[end_actor_name])){
			    return originYear;
			}
		}
		auto it_cast = m->casts.begin();
		auto ed_cast = m->casts.end();
		while(it_cast!=ed_cast){
			merge((m->casts.begin())->second, it_cast->second);
			it_cast++;
		}
		
		originYear = m->year;
		it++;
	}

	return 9999;
}

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
    string movieID = to_string(movie_year) + movie_title;

    if(oByYear_Movies.count(movieID) == 0){
        Movie* mov = new Movie(movie_title,movie_year);
        oByYear_Movies[movieID] = mov;
    }

    //Then take out the entity through their index
    ActorNode* currActorNode = Actors[actor_name];
    Movie* mov = oByYear_Movies[movieID];

    /*
    Finally we need to insert edges between nodes according to this information
    The main idea is centered at the movie in the info, and make connections between current actor
    with all the actors in the cast list of the movie.
    The graph is undirected, but we can see it as two directed edges retrospectively, so we need to add both edges which take both nodes as start node, seperatively;
    Otherwise, if we just initialize one edge, then the start point and end point will be mixed
    we cannot directly use .end to indicate the next node when bfs
    */
    // auto it = mov->casts.begin();
    // auto ed = mov->casts.end();
    // while(it != ed){
    //     ActorNode* endActorNode = it->second;
    //     insertEdgeForStart(currActorNode, endActorNode, mov);
    //     insertEdgeForStart(endActorNode, currActorNode, mov);
    //     it++;
    // }

    //After all the connections have been made, add current actor on the cast list. To avoid self cycle.
    mov->casts[actor_name] =currActorNode;

    //
    //ordered_years.insert(movie_year);
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