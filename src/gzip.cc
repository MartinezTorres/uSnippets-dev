////////////////////////////////////////////////////////////////////////
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 -lz

#include <uSnippets/gzip.hpp>
#include <uSnippets/log.hpp>
using namespace std;
using uSnippets::Log;
int main() {
	
	std::string data;
	
	std::string in;
	for (int i=0; i<100000; i++) { in += string("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz").substr(rand()%50, rand()%10); }
	Log(0) << "start coding";
	for (int level=1; level<10; level++) {
		std::string compressed = uSnippets::Codecs::GZip::code(in, level);
		Log(0) << "Compressed on level " << level << " with ratio: " << double(compressed.size())/in.size();
		std::string in2 = uSnippets::Codecs::GZip::decode(compressed);
		Log(0) << "Decompression: " << (in2==in?"success":"failed");		
	}
	
	return 0;
}
