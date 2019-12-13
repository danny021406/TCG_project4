#include "node.h"
#include <algorithm>
#include <math.h>
#include <cstdlib>

#define BLACK 0
#define WHITE 1
#define WEIGHT 0.75

ostream & operator << (ostream &out, const pair<int,int> &p) {
    out << p.first << " " << p.second << " ";
    return out;
}

class mcts{
public:
	bool color;
	node* root;
	vector<node*> path;
	board mboard;

	double getscore( node* nodeptr, int child) {
		//cerr << "in getscore ";
		node *tmp = (nodeptr->childptr)+child;
		//cerr <<  (double)tmp->win/(double)(tmp->count+1) << "  " << WEIGHT * sqrt(log((double)nodeptr->count)/(double)(tmp->count+1)) << " ";
		return (double)tmp->win/(double)(tmp->count+1) + WEIGHT * sqrt(log((double)(nodeptr->count+1))/(double)(tmp->count+1));
	}

	node* getbestchild(node* nodeptr) {
		//cerr << "in getbestchild\n";
		if ( nodeptr->csize == 0 ) return NULL;
		double ans = -1 ;
		vector<double> tmpvec; 
		vector<int> candidate;
		
		for ( int i = 0 ; i < nodeptr->csize ; i++ ) {
			//cerr << "i = " << i << " " ;
			double tmp = getscore(nodeptr,i);
			//cerr << tmp << endl;
			tmpvec.push_back(tmp);
			ans = max(tmp,ans);
		}
		//cerr << ans << " " << nodeptr->csize << endl;
		for ( int i = 0 ; i < nodeptr->csize ; i++ ) {
			//cerr << ( ans - tmpvec[i] ) << " " << endl;
			if ( ( ans - tmpvec[i] ) < (double)0.001 ) candidate.push_back(i);
		}
		//cerr << candidate.size() << endl;
		//cerr << "before return\n";
		int k = (rand()%candidate.size());
		node* tmp = (nodeptr->childptr) + candidate[(rand()%candidate.size())];
		//cerr << tmp->place << endl ;
		return tmp;
	}


	node* select(board& b) {
		path.clear();
		node* nodeptr = root;
		path.push_back(nodeptr);

		while( nodeptr->childptr != NULL && nodeptr->csize != 0 ) {
			nodeptr = getbestchild(nodeptr);
			//cerr << "after return\n";
			path.push_back(nodeptr);
			b.add(nodeptr->place.first,nodeptr->place.second,nodeptr->color);
		}
		return nodeptr;
	}

	void expand(node* ptr, board& b) {
		if ( ptr->csize == 0 ) {
			(*ptr).expansion(b);
		}
	}

	bool simulate(board& b, bool color) {
		bool flag = true;
		vector<pair<int,int> > my;
		vector<pair<int,int> > promy;
		
		bool nowcolor = !color;
		while(1) {
			pair<int,int> choose;
			b.get_avaiable(nowcolor,my,promy);
			if ( promy.size() != 0 ) choose = promy[rand()%promy.size()];
			else if( my.size() != 0 ) choose = my[rand()%my.size()];
			else flag = false;

			if ( flag == false ) break;
			else b.update(choose.first,choose.second,nowcolor,0);
			nowcolor = !nowcolor;
			my.clear();
			promy.clear();
		}

		return !nowcolor;
	}

	void update(bool winer) {
		int win ;
		if ( winer == color ) win = 1;
		else win = 0;
		for ( int i = 0 ; i < path.size() ; i++ ) {
			path[i]->count++;
			path[i]->win += win;
		}
	}

	void run() {
		srand(time(NULL));
		//cerr << "init board  " ;
		board b = mboard; 
		node* ptr;
		//cerr << "select  " ;
		ptr = select(b);
		b.set_air();
		//cerr << "expand  " ;
		expand(ptr,b);
		//cerr << "simulate  " ;
		bool winner = simulate(b,ptr->color);
		//cerr << "winner: " << winner << "  " ;
		//cerr << "update  " ;
		update(winner);
		//cerr << "fin  \n" ;
		int vdd;
		//cin >> vdd;
	}

	void reset(board& b, bool color) {
		cerr << "now color " <<  color << endl; 
		this->color = color;
		mboard = b ;
		b.show();
		//b.set_air();
		//b.show_checker();
		//b.show_air();
		root = new node;
		root->initnode(pair<int,int>(0,0),!color);
		root->expansion(b);
		// for ( int i = 0 ; i < root->csize ; i++ ) {
		// 	cerr << ((root->childptr)+i)->place << "   " ; 
		// }
		path.clear();
	}

	void clear() {
		if( root != NULL) delete root;
	}

	pair<int,int> getbestmove() {
		int ans = 0 , pos = 0 ;
		for ( int i = 0 ; i < root->csize ; i++ ) {
			cerr << root->childptr[i].count << "/" << root->childptr[i].win << " " ;
			if ( root->childptr[i].count > ans ) {		
				pos = i ;
				ans = root->childptr[i].count;
			}
		}
		//cerr << endl;
		return root->childptr[pos].place;
	}

};
