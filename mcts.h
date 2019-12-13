#include "node.h"
#include <algorithm>
#include <math.h>
#include <cstdlib>
#include <map>

extern map<pair<int,int> , vector<node*> > bpath;
extern map<pair<int,int> , vector<node*> > wpath;

ostream & operator << (ostream &out, const pair<int,int> &p) {
    out << p.first << " " << p.second << " ";
    return out;
}

class mcts{
public:
	bool color;
	node* root;
	vector<node*> path;
	// map< pair<int,int>, vector<node*> > bpath;
	// map< pair<int,int>, vector<node*> > wpath;
	board mboard;

	double getscore( node* nodeptr, int child) {
		//cerr << "in getscore ";
		node *tmp = (nodeptr->childptr)+child;
		//cerr <<  (double)tmp->win/(double)(tmp->count+1) << "  " << WEIGHT * sqrt(log((double)nodeptr->count)/(double)(tmp->count+1)) << " ";
		double ucbwin = (double)tmp->win/(double)(tmp->count+1) ;
		double exporation = WEIGHT * sqrt(log((double)(nodeptr->count+1))/(double)(tmp->count+1));
		if ( RAVE ) {
			double ravewin = (double)tmp->ravewin/(double)(tmp->ravecount+1) ;
			double ravepercent = (double)(tmp->ravecount+1)/((double)(tmp->ravecount+1)+(double)(tmp->count+1));
			double ucbpercent = (double)(tmp->count+1)/((double)(tmp->ravecount+1)+(double)(tmp->count+1));
			return ravepercent*ravewin + ucbpercent*ucbwin + exporation ;
		} else return ucbwin + exporation;
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

	bool simulate(board& b, bool c) {
		bool flag = true;
		vector<pair<int,int> > my;
		vector<pair<int,int> > promy;
		
		bool nowcolor = !c;
		while(1) {
			pair<int,int> choose;
			b.get_avaiable(nowcolor,my,promy);
			if ( promy.size() != 0 ) choose = promy[rand()%promy.size()];
			else if( my.size() != 0 ) choose = my[rand()%my.size()];
			else flag = false;

			if ( flag == false ) break;
			else b.update(choose.first,choose.second,nowcolor,1);
			nowcolor = !nowcolor;
			my.clear();
			promy.clear();
		}

		return !nowcolor;
	}

	void update(bool winner) {
		for ( int i = 0 ; i < path.size() ; i++ ) {
			path[i]->update(winner);
			if ( RAVE ) {
				if ( path[i]->color == 0 ) {
					for ( auto j : bpath[path[i]->place] ) {
						j->update_rave(winner);
					}
				} else {
					for ( auto j : wpath[path[i]->place] ) {
						j->update_rave(winner);
					}
				}
			}
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
		
		this->color = color;
		mboard = b ;
		bpath.clear();
		wpath.clear();

		if ( RAVE ) {
			for ( int i = 1 ; i < 10 ; i++ ) {
				for ( int j = 1 ; j < 10 ; j++ ) {
					if ( b.checker(i,j,0) ) {
						vector<node*> tmp;
						bpath[pair<int,int>(i,j)] = tmp;
					}
					if ( b.checker(i,j,1) ) {
						vector<node*> tmp;
						wpath[pair<int,int>(i,j)] = tmp;
					}
				}
			}
		}

		root = new node;
		root->initnode(pair<int,int>(0,0),!color);
		root->expansion(b);
		// if ( RAVE ) {
		// 	for ( int i = 1 ; i < 10 ; i++ ) {
		// 		for ( int j = 1 ; j < 10 ; j++ ) {
		// 			if ( b.checker(i,j,0) ) {
		// 				cerr << i << " " << j << " " << bpath[pair<int,int>(i,j)].size() << endl;
		// 			}
		// 			if ( b.checker(i,j,1) ) {
		// 				cerr << i << " " << j << " " << wpath[pair<int,int>(i,j)].size() << endl;
		// 			}
		// 		}
		// 	}
		// }
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
			cerr << root->childptr[i].count << "/" << root->childptr[i].win << ":" << root->childptr[i].ravecount << "/" << root->childptr[i].ravewin << " " ; 
			if ( root->childptr[i].count > ans ) {		
				pos = i ;
				ans = root->childptr[i].count;
			}
		}
		//cerr << endl;
		return root->childptr[pos].place;
	}

};
