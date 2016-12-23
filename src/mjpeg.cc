////////////////////////////////////////////////////////////////////////
// MJPEG encoder
//
// Manuel Martinez (salutte@gmail.com)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 -lboost_system -pthread `pkg-config opencv --cflags --libs` -lturbojpeg

#include <uSnippets/mjpeg.hpp>

#include <opencv/highgui.h>
#include <iostream>

using namespace std;
using namespace uSnippets;

int main(int argc, char *argv[]) {
	
	int q = 95;
	if (argc==2) q = atoi(argv[1]);

	Mjpeg mjpg(8080, q);

	cv::VideoCapture cap(0);
	cv::Mat3b img;
	while (true) {
		cap >> img;
		mjpg.add(img);
		
		cerr << img.rows << endl;
	}
	
	return 0;
}

// Thanks for R.T.F.C.
