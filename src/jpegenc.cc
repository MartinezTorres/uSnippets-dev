////////////////////////////////////////////////////////////////////////
// micro JPEG encoder
//
// Manuel Martinez (salutte@gmail.com)
// inspired by: jpge from Rich Geldreich
//
// Stripped down JPEG encoder with the base essentials. Compression is
// fixed to baseline YUV 411. Suit yourself to modify it for your own
// needs.
// 
// The goal of this library is to provide a small but clear baseline jpeg
// implementation for teaching and experimentation purposes.
//
// license: LGPLv3
//
// FLAGS: -g `pkg-config opencv --cflags --libs`

#include <uSnippets/jpegenc.hpp>


#include <opencv/highgui.h>
#include <fstream>
#include <iostream>

using namespace std;
using namespace uSnippets;

int main(int argc, char *argv[]) {
	
	if (argc<3) {
		cout << "Use: ./ujpge <file.png> <Q>" << endl;
		cout << "E.g. ./ujpge lena.png 50" << endl;
		return -1;
	}

	cv::Mat3b img = cv::imread(argv[1]);
	
	ofstream of("ujpg.out.jpg");

	of << JpegEnc::encode(img.data, img.cols, img.rows, atoi(argv[2]));
	
	return 0;
}

// Thanks for R.T.F.C.

