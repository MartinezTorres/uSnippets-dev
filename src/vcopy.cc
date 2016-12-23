////////////////////////////////////////////////////////////////////////
// minimal copy program with verification
//
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11

#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>

using namespace std;

int main(int argc, char *argv[]) {
	
	ifstream in(argv[1], ios_base::binary);
	{ 
		ofstream out(argv[2], ios_base::binary | ios_base::out);
	}
	fstream out(argv[2], ios_base::binary | ios_base::in | ios_base::out);

	cout << argc << " " << in.is_open() << " " << out.is_open() << endl;

	if (argc != 3 or not in.is_open() or not out.is_open() ) {
		
		cout << "Usage: vcopy in out" << endl;
		exit(-1);
	}
	
	in.seekg (0, in.end);
	uint64_t length = in.tellg();
	in.seekg (0, in.beg);
	uint64_t done = 0;
    
	static const int BUFFSIZE = 1*(1<<15);
	char buffer[BUFFSIZE];
	char vbuffer[BUFFSIZE];

	int tries = 3;
	while (in) {
		
		streampos pos = in.tellg();
		in.read(buffer,BUFFSIZE);
		streamsize sz = in.gcount();

		out.seekp(pos);
		out.write(buffer,sz);
		out.flush();
		if (system("sync")) break;
		out.seekg(pos);
		
		out.read(vbuffer,sz);
		if (strncmp(buffer,vbuffer,sz)) {
			cout << "Error at: " << pos << endl;
			if (not tries--) exit(-1);
			in.seekg(pos);
		} else tries = 4;
		
		if (round(100*(done)/length) != round(100*(done+sz)/length))
			cout << round(100*(done+sz)/length) << "%" << endl;

		cout << "." << flush;

		done += sz;
	}
	
	
	return 0;
}
