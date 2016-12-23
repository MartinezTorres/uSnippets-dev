////////////////////////////////////////////////////////////////////////
// minimal kdtree (with aNN) code
//
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 -O3

#include <uSnippets/kdtree.hpp>
#include <iostream>

using namespace std;


int main() {

	// Fast
	{
		KDTree<std::array<double,4>> kdtree;
		
		vector<array<double, 4>> data;
		for (int i=0; i<100000; i++) {
			data.push_back({{rand()%256+0., rand()%256+0., rand()%256+0., rand()%256+0.}});
			kdtree.add(data.back());
		}
		
		for (int i=0; i<5; i++) {
			for (int j=0; j<1000; j++)
				auto R = kdtree.getNN(data[rand()%data.size()], 10);

			auto R = kdtree.getNN(data[rand()%data.size()], 5);
			for (auto &r : R)
				cout << r.first.p[0] << " " << r.second << endl;
		}
	}
	
	// Not so fast
	{

		KDTree<> kdtree;
		
		vector<vector<double>> data;
		for (int i=0; i<100000; i++) {
			data.push_back({{rand()%256+0., rand()%256+0., rand()%256+0., rand()%256+0.}});
			kdtree.add(data.back());
		}
		
		for (int i=0; i<5; i++) {
			for (int j=0; j<1000; j++)
				auto R = kdtree.getNN(data[rand()%data.size()], 10);

			auto R = kdtree.getNN(data[rand()%data.size()], 5);
			for (auto &r : R)
				cout << r.first.p[0] << " " << r.second << endl;
		}
	}
	
	return 0;
}
