//#include "bitboard.h"
#include <set>

#define BLACK 0
#define WHITE 1

using namespace std;

class board{
public:
	int air[11][11];
	bool b[11][11];
	bool hole[11][11];
	bool doneboard[11][11];
	bool doneboard2[11][11];
	set<pair<int,int> > updateset;
	//bool [9][9];

	board() {
		reset_all();
	}

	void operator =(const board& b) {

		for ( int i = 1 ; i < 10 ; i++ ) {
			for ( int j = 1 ; j < 10 ; j++ ) {
				this->hole[i][j] = b.hole[i][j];
				this->b[i][j] = b.b[i][j];
			}
		} 
	}

	void holeupdate(int x, int y, bool color) {
		doneboard[x][y] = true;
		if ( x+1 != 10 && hole[x+1][y] == false && b[x+1][y] == color && doneboard[x+1][y] == false ) holeupdate(x+1,y,color);
		if ( x-1 !=  0 && hole[x-1][y] == false && b[x-1][y] == color && doneboard[x-1][y] == false ) holeupdate(x-1,y,color);
		if ( y+1 != 10 && hole[x][y+1] == false && b[x][y+1] == color && doneboard[x][y+1] == false ) holeupdate(x,y+1,color);
		if ( y-1 !=  0 && hole[x][y-1] == false && b[x][y-1] == color && doneboard[x][y-1] == false ) holeupdate(x,y-1,color);
		if ( x+1 != 10 && hole[x+1][y] == true ) updateset.insert(pair<int,int>(x+1,y));
		if ( x-1 !=  0 && hole[x-1][y] == true ) updateset.insert(pair<int,int>(x-1,y));
		if ( y+1 != 10 && hole[x][y+1] == true ) updateset.insert(pair<int,int>(x,y+1));
		if ( y-1 !=  0 && hole[x][y-1] == true ) updateset.insert(pair<int,int>(x,y-1));
	}

	void holeupdate2(int x, int y, bool color, int v) {
		doneboard[x][y] = true;
		air[x][y] = v;
		if ( x+1 != 10 && hole[x+1][y] == false && b[x+1][y] == color && doneboard[x+1][y] == false ) holeupdate2(x+1,y,color,v);
		if ( x-1 !=  0 && hole[x-1][y] == false && b[x-1][y] == color && doneboard[x-1][y] == false ) holeupdate2(x-1,y,color,v);
		if ( y+1 != 10 && hole[x][y+1] == false && b[x][y+1] == color && doneboard[x][y+1] == false ) holeupdate2(x,y+1,color,v);
		if ( y-1 !=  0 && hole[x][y-1] == false && b[x][y-1] == color && doneboard[x][y-1] == false ) holeupdate2(x,y-1,color,v);
	}

	void holeupdate3(int x, int y, bool color, int v) {
		doneboard2[x][y] = true;
		doneboard[x][y] = true;
		air[x][y] = v;
		if ( x+1 != 10 && hole[x+1][y] == false && b[x+1][y] == color && doneboard[x+1][y] == false ) holeupdate3(x+1,y,color,v);
		if ( x-1 !=  0 && hole[x-1][y] == false && b[x-1][y] == color && doneboard[x-1][y] == false ) holeupdate3(x-1,y,color,v);
		if ( y+1 != 10 && hole[x][y+1] == false && b[x][y+1] == color && doneboard[x][y+1] == false ) holeupdate3(x,y+1,color,v);
		if ( y-1 !=  0 && hole[x][y-1] == false && b[x][y-1] == color && doneboard[x][y-1] == false ) holeupdate3(x,y-1,color,v);
	}

	void update(int x, int y, bool color, bool mode) {
		bool op_color = !color ;
		b[x][y] = color;
		hole[x][y] = false;

		if ( hole[x+1][y] == false && b[x+1][y] == op_color ) {
			updateset.clear();
			reset_doneboard();
			holeupdate(x+1,y,op_color);
			reset_doneboard();
			if ( mode )holeupdate2(x+1,y,op_color,updateset.size());
			else holeupdate3(x+1,y,op_color,updateset.size());
		}
		if ( hole[x-1][y] == false && b[x-1][y] == op_color ) {
			updateset.clear();
			reset_doneboard();
			holeupdate(x-1,y,op_color);
			reset_doneboard();
			if ( mode )holeupdate2(x-1,y,op_color,updateset.size());
			else holeupdate3(x-1,y,op_color,updateset.size());
		}
		if ( hole[x][y+1] == false && b[x][y+1] == op_color ) {
			updateset.clear();
			reset_doneboard();
			holeupdate(x,y+1,op_color);
			reset_doneboard();
			if ( mode )holeupdate2(x,y+1,op_color,updateset.size());
			else holeupdate3(x,y+1,op_color,updateset.size());
		}	
		if ( hole[x][y-1] == false && b[x][y-1] == op_color ) {
			updateset.clear();
			reset_doneboard();
			holeupdate(x,y-1,op_color);
			reset_doneboard();
			if ( mode )holeupdate2(x,y-1,op_color,updateset.size());
			else holeupdate3(x,y-1,op_color,updateset.size());
		}

		updateset.clear();
		reset_doneboard();
		holeupdate(x,y,color);
		reset_doneboard();
		if ( mode )holeupdate2(x,y,color,updateset.size());
		else holeupdate3(x,y,color,updateset.size());

	}

	void reset_all() {
		reset_air();
		reset_hole();
	}

	void reset_doneboard() {
		memset(&doneboard,false, sizeof(doneboard));
	}

	void reset_doneboard2() {
		memset(&doneboard2,false, sizeof(doneboard2));
	}

	void reset_air() {
		memset(&air,0, sizeof(air));
	}

	void reset_hole() {
		memset(&hole,1, sizeof(hole));
	}

	void set_air() {
		reset_doneboard2();
		for ( int i = 1 ; i < 10 ; i++ ) {
			for ( int j = 1 ; j < 10 ; j++ ) {
				if ( doneboard2[i][j] == false && hole[i][j] == false ) {
					update(i,j,b[i][j],false);
				}
			}
		}
	}

	bool checker(int x, int y, bool color) {
		if ( hole[x][y] == false ) return false;

		int holecnt = 0 ;
		bool op_color = !color;
		if ( x+1 != 10 && hole[x+1][y] == true ) holecnt++;
		if ( x-1 !=  0 && hole[x-1][y] == true ) holecnt++;
		if ( y+1 != 10 && hole[x][y+1] == true ) holecnt++;
		if ( y-1 !=  0 && hole[x][y-1] == true ) holecnt++;

		if ( holecnt == 0 ) {
			if ( x+1 != 10 && hole[x+1][y] == false && b[x+1][y] == op_color ){ if ( air[x+1][y] == 1 ) return false; }
			if ( x-1 !=  0 && hole[x-1][y] == false && b[x-1][y] == op_color ){ if ( air[x-1][y] == 1 ) return false; }
			if ( y+1 != 10 && hole[x][y+1] == false && b[x][y+1] == op_color ){ if ( air[x][y+1] == 1 ) return false; }
			if ( y-1 !=  0 && hole[x][y-1] == false && b[x][y-1] == op_color ){ if ( air[x][y-1] == 1 ) return false; }

			bool flag = false;
			if ( x+1 != 10 && hole[x+1][y] == false && b[x+1][y] == color ){ if ( air[x+1][y] != 1 ) flag = true; }
			if ( x-1 !=  0 && hole[x-1][y] == false && b[x-1][y] == color ){ if ( air[x-1][y] != 1 ) flag = true; }
			if ( y+1 != 10 && hole[x][y+1] == false && b[x][y+1] == color ){ if ( air[x][y+1] != 1 ) flag = true; }
			if ( y-1 !=  0 && hole[x][y-1] == false && b[x][y-1] == color ){ if ( air[x][y-1] != 1 ) flag = true; }

			if ( flag == true ) return true;
			return false;
		}else {
			if ( x+1 != 10 && hole[x+1][y] == false && b[x+1][y] == op_color ){ if ( air[x+1][y] == 1 ) return false; }
			if ( x-1 !=  0 && hole[x-1][y] == false && b[x-1][y] == op_color ){ if ( air[x-1][y] == 1 ) return false; }
			if ( y+1 != 10 && hole[x][y+1] == false && b[x][y+1] == op_color ){ if ( air[x][y+1] == 1 ) return false; }
			if ( y-1 !=  0 && hole[x][y-1] == false && b[x][y-1] == op_color ){ if ( air[x][y-1] == 1 ) return false; }
			return true;
		}
	}

	void add(int x, int y, bool color) {
		b[x][y] = color;
		hole[x][y] = false;
	}

	void get_avaiable(bool color, vector<pair<int,int> >& my, vector<pair<int,int> >& promy ) {
		bool op_color = !color;
		for ( int i = 1 ; i < 10 ; i++ ) {
			for ( int j = 0 ; j < 10 ; j++ ) {
				bool mycolorc = checker(i,j,color);
				bool opcolorc = checker(i,j,op_color);
				if ( mycolorc == true && opcolorc == true ) promy.push_back( pair<int,int>(i,j) );
				else if ( mycolorc == true ) my.push_back( pair<int,int>(i,j) );
			}
		}
	}

	void show() {
		cerr <<  "show my board: \n";
		for ( int i = 1 ; i < 10 ; i++ ) {
			for ( int j = 1 ; j < 10 ; j++ ) {
				if ( hole[i][j] == false ) cerr <<  b[i][j] << " " ;
				else cerr << "  ";
			}
			cerr << endl;
		}
	}

	void show_cout() {
		cout <<  "show my board: \n";
		for ( int i = 1 ; i < 10 ; i++ ) {
			for ( int j = 1 ; j < 10 ; j++ ) {
				if ( hole[i][j] == false ) cout <<  b[i][j] << " " ;
				else cout << "  ";
			}
			cout << endl;
		}
	}

	void show_air() {
		cout <<  "show my air board: \n";
		for ( int i = 1 ; i < 10 ; i++ ) {
			for ( int j = 1 ; j < 10 ; j++ ) {
				if ( hole[i][j] == false ) cerr <<  air[i][j] << " " ;
				else cerr << "  ";
			}
			cerr << endl;
		}
	}

	void show_checker() {
		cerr << "black\n"; 
		for ( int i = 1 ; i < 10 ; i++ ) {
			for ( int j = 1 ; j < 10 ; j++ ) {
				if ( hole[i][j] == true ) cerr <<  checker(i,j,0) << " " ;
				else cerr << "  ";
			}
			cerr << endl;
		}
		cerr << "white\n";
		for ( int i = 1 ; i < 10 ; i++ ) {
			for ( int j = 1 ; j < 10 ; j++ ) {
				if ( hole[i][j] == true ) cerr <<  checker(i,j,1) << " " ;
				else cerr << "  ";
			}
			cerr << endl;
		}
	}

};