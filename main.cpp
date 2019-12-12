#include <iostream>
#include <stdlib.h>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <time.h>
#include "mcts.h" 

using namespace std;

#define timelimit 1
#define FUCKITSELF true

void toyz() {
	board bk;
	bool flag = true ;
	int cnt = 20;
	while(cnt--) {
		int x = rand()%9 + 1 ;
		int y = rand()%9 + 1 ;
		bk.add(x,y,flag);
		cout << "pos: " << x << " " << y << endl;
		bk.show();
		flag = !flag;
	}
	cout << "board\n" ;
	bk.show();
	cout << "air\n";
	bk.set_air();
	bk.show_air();
	bk.show_checker();
}



pair<int,int> GTPstringtoint(string s)
{
	if(s[0]>='a'&&s[0]<='z')
	{
		int x=s[0]-'a';
		if(s[0]>'i')x--;
		int y=s[1]-'1';
		return pair<int,int>(x+1,y+1);
	}else
	{
		int x=s[0]-'A';
		if(s[0]>'I')x--;
		int y=s[1]-'1';
		return pair<int,int>(x+1,y+1);
	}
	
}

string inttoGTPstring(pair<int,int> pos) {
	string s = "A1";
    s[0]+= (pos.first-1);
    if(s[0]>='I')s[0]++;
    s[1] += (pos.second-1) ;
    return s;
}

mcts tree;

int main () {

	string command, c, position;
	pair<int,int> best_move;
	bool color , exit;
	board mainboard;
	int x, y;
	int cnt;
	if ( !FUCKITSELF ) {
		while( cin >> command ) {
			//cnt = 10000;

			if ( command == "play" || command == "p" ) { // PLAY COLOR POSITION

				cin >> c >> position;
				if( c[0] == 'b' || c [0] == 'B' ) color = 0 ;
				else color = 1 ;
				pair<int,int> tmp = GTPstringtoint(position);
				x = tmp.first;
				y = tmp.second;
				mainboard.update( x,y,color,0 );
				cout << "=" << endl << endl ;

			}else if ( command[0] == 'g' || command == "reg_genmove" ){ // G COLOR
				// time
				int e , st;
				e = st = clock();
				cnt = 20000;
				
				// color
				cin >> c ;
				if( c[0] == 'b' || c [0] == 'B' ) color = 0 ;
				else color = 1 ;

				tree.reset(mainboard,color);

				// resign
				bool resign = true;
				for ( int i = 1 ; i < 10 ; i++ ) {
					for ( int j = 1 ; j < 10 ; j++ ) {
						if ( mainboard.checker(i,j,color) ) {
							resign = false;
							break;
						}
						if ( resign == false ) break;
					}
				}
				if ( resign == true ) {
					exit = true;
					cout << "=resign" << endl << endl;
					continue;
				}
				cerr << "msts run\n" ; 
				//mcts run
				while(cnt--){
					cerr << "cnt: " << cnt << endl;
					tree.run();
					e = clock();
				}
				
				cerr << "after run\n";
				//best move 

				best_move = tree.getbestmove();
				mainboard.update(best_move.first,best_move.second,color,0);
				//output
				cerr << "simulation time : " << (double)(e-st) / 1000.0 << endl;
				cout << "=" << inttoGTPstring(best_move) << endl << endl;

				tree.clear();
			}else if ( command == "resign" ) {
				exit = true;
				cerr << "nogo win\n";
			}else if ( command == "clear" ) {
				mainboard.reset_all();
			}

			mainboard.show();

			if ( exit ) break;

		}
	}else {
		while(1) {
			int wer;
			//cin >> wer;



			int e , st;
			e = st = clock();
			

			tree.reset(mainboard,color);

			// resign
			bool resign = true;
			for ( int i = 1 ; i < 10 ; i++ ) {
				for ( int j = 1 ; j < 10 ; j++ ) {
					if ( mainboard.checker(i,j,color) ) {
						resign = false;
						break;
					}
					if ( resign == false ) break;
				}
			}
			if ( resign == true ) {
				exit = true;
				cout << "=resign" << endl << endl;
				break;
				continue;
			}
			//cerr << "msts run\n" ; 
			//mcts run
			while(1){
				//cerr << "cnt: " << cnt << endl;
				tree.run();
				tree.run();
				tree.run();
				tree.run();
				tree.run();
				tree.run();
				tree.run();
				tree.run();
				tree.run();
				tree.run();
				e = clock();
				if ( ((double)(e-st) / CLOCKS_PER_SEC) > 0.9 ) break;
			}
			
			//cerr << "after run\n";
			//best move 

			best_move = tree.getbestmove();
			mainboard.update(best_move.first,best_move.second,color,0);
			//output
			cerr << "total node: " << endl;
			cerr << "simulation time : " << ((double)(e-st) / CLOCKS_PER_SEC) << endl;
			cout << "=" << inttoGTPstring(best_move) << endl << endl;

			tree.clear();
			//mainboard.show_cout();
			//mainboard.show();
			//mainboard.show_air();

			color = !color;
			if ( resign == true ) { 
				cout << "winner = " << color << endl;
				break;
			}
			
		}
	}
	cerr << "end of process\n";

}