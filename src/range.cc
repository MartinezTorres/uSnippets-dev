////////////////////////////////////////////////////////////////////////
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 -pthread -lboost_system

#include <uSnippets/range.hpp>

#include <iostream>
#include <fstream>

using namespace std;


using namespace std;


int main(int argc, char *argv[]) {
	
	if (argc>1) {
		
		std::ifstream is(argv[1]);
		vector<uint16_t> in;
		while (is) in.push_back(uint8_t(is.get()));

		
		for (int l=0; l<20; l+=2) {
			
			cout << "Level " << l << endl;

			std::string c = uSnippets::Codecs::Range::encode(in,256,l);  
			cout << c.size() << " " << float(c.size())/float(in.size()) << endl;

			std::vector<uint16_t> out = uSnippets::Codecs::Range::decode(c,256);
			cout << out.size() << " " << (out==in) << endl;

		}
		
		return -1;
	}
	
	for (int N=1; N<=256; N=N*2) {
		cout << "Nº different symbols: " <<  N << endl;
		static const int S=1<<24; 
		
		std::vector<int> prob(N);
		for (int i=0; i<N; i++) prob[i] = rand()%1024;

		std::vector<uint16_t> in(S);
		for (int i=0; i<S; i++) do in[i]=rand()%N; while (rand()%1024 > prob[in[i]]);
			
		cout << "Got Input data of size: " << in.size() << endl;

		std::string inc = uSnippets::Codecs::Range::encode(in,N,6);  
		cout << "Compressed with history = 6: " << inc.size() << endl;

		inc = uSnippets::Codecs::Range::encode(in,N,0);  
		cout << "Compressed with history = 0: " << inc.size() << endl;

		std::vector<uint16_t> out = uSnippets::Codecs::Range::decode(inc, N);
		cout << "Decoded to: " << out.size() << " " << (in==out) << endl;
		cout << endl;
	}
	
	return 0;
}
