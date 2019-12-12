#include "board.h"

class node {
public:
	int csize;
	int count;
	int win;
	bool color;
	pair<int,int> place;
	node* childptr;

	// node(bool color) {
	// 	this->color = color;
	// }

	~node() {
	    if( childptr != NULL) { delete [] childptr ; childptr = NULL; }
	}

	void initnode(pair<int,int> pos, bool color ) {
		this->color = color;
		place = pos;
		count = 0 ;
		win = 0 ;
		csize = 0 ;
		childptr = nullptr;
	}

	void expansion(board &b) {
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
		//cerr << "expandsize " << csize << endl;
		if ( csize == 0 ) return ;
		childptr = new node[csize];
		for( int i = 0 ; i < placevec.size() ; i++ ) {
			childptr[i].initnode(placevec[i],op_color);
		}	
	}


};