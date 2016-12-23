////////////////////////////////////////////////////////////////////////
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
// FLAGS: -g -std=c++11
#include <uSnippets/object.hpp>

#include <iostream>
using namespace std;
using namespace uSnippets;

int main() {

	object o = "3";
	o = int(o) - 10./3;
	cout << o + " WoW " + 3 << endl;

	o = "";
	o << "one" << " " <<  "two" << " " << 3;
	cout << o << endl;

	string s1, s2;
	o >> s1 >> s2;
	cout << s1 << " " << s2 << endl;
	cout << o << endl;
}
