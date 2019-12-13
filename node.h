#include "board.h"
#include <map>
#include <vector>



class node {
public:
	int csize;
	int count;
	int ravecount;
	int win;
	int ravewin;
	bool color;
	pair<int,int> place;
	node* childptr;
	~node();
	void initnode(pair<int,int> pos, bool color );
	void expansion(board &b);
	void update(bool winner);
	void update_rave(bool winner);
};

map<pair<int,int> , vector<node*> > bpath;
map<pair<int,int> , vector<node*> > wpath;

node::~node() {
    if( childptr != NULL) { delete [] childptr ; childptr = NULL; }
}

void node::initnode(pair<int,int> pos, bool color ) {
	this->color = color;
	place = pos;
	ravecount = 0 ;
	count = 0 ;
	ravewin = 0 ;
	win = 0 ;
	csize = 0 ;
	childptr = nullptr;

}

void node::expansion(board &b) {
	vector<pair<int,int> > placevec;
	bool op_color = !color;
	csize = 0 ;
	for( int i = 1 ; i < 10 ; i++ ) {
		for ( int j = 1 ; j < 10 ; j++ ) {
			if ( b.checker(i,j,op_color) == true ){
				csize++;
				placevec.push_back(pair<int,int>(i,j));
			}
		}
	}

	if ( csize == 0 ) return ;
	childptr = new node[csize];
	for( int i = 0 ; i < placevec.size() ; i++ ) {
		childptr[i].initnode(placevec[i],op_color);
		if ( RAVE ) {
			if ( op_color == 0 ) {
				//cerr << "bb " ;
				bpath[placevec[i]].push_back(childptr+i);
			} else {
				//cerr << "ww " ;
				wpath[placevec[i]].push_back(childptr+i);
			}
		}		
	}	
}

void node::update(bool winner) {
	if ( winner == color ) win++;
	count++; 
}

void node::update_rave(bool winner) {
	if ( winner == color ) ravewin++;
	ravecount++; 
}
