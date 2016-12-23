////////////////////////////////////////////////////////////////////////
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// To test, in one terminal run: nc.traditional -l -p 8888 -e bin/comm
// And in other: nc.traditional -e bin/comm localhost 8888
//
// FLAGS: -g -std=c++11 

#include <uSnippets/time.hpp>

#include <iostream>
using namespace std;
using namespace uSnippets;

int main() {
	
	time_point tp = now();
	
	tp = tp + 10_s;
	
	cout << print(tp+10_s) << endl;
}
