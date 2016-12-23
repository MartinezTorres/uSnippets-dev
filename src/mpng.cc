////////////////////////////////////////////////////////////////////////
// PNG coder with metadata support
//
// Manuel Martinez (manuel.martinez@kit.edu)
// Image obtained from: http://sipi.usc.edu/database/?volume=misc
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs` -lpng

#include <uSnippets/mpng.hpp>

using namespace std;
using namespace uSnippets;

int main() {

	srand(time(0));
	
	MPNG png("data/images/sipimisc/4.2.04.png");
	png.write("data/tmp/lena.png");

	cout << endl << "Size: " << png.cols << "x" << png.rows << endl;

	png["seed"] = rand()%1024;
	cout << endl << "Add metadata \"seed\" = " << png["seed"] << endl;
	
	cout << endl << "Current metadata" << endl;
	for (uint i=0; i<png.metadata.size(); i++)
		cout << png.metadata[i].first << ": " << png.metadata[i].second << endl;
	
	png.write();
	
	cout << endl << "png size: " << object(png).size() << endl;
}
