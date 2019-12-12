#include <iostream>
#include <vector>

using namespace std;

int main () {
	bool doneboard[11][11] ;
	memset(&doneboard,1, sizeof(doneboard));
	for ( int i = 0 ; i < 11 ; i++ ) {
		for ( int j = 0 ; j < 11 ; j++ ) {
			cout << doneboard[i][j] << " ";
		}
		cout << endl;
	}
}