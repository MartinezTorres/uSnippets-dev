////////////////////////////////////////////////////////////////////////
// OpenNI capture example
//
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs` ${OPENNI2_REDIST}/libOpenNI2.so
#include <uSnippets/openni.hpp>
#include <opencv/highgui.h>

using namespace std;
using namespace uSnippets;

int main() {
	
	OpenNI openni;

//	cv::namedWindow("color",0);
//	openni.colorCB([](cv::Mat3b img){cv::imshow("color",img);});

	
	openni.depthCB([&](cv::Mat1s img){ cerr << "iup" << endl; cv::namedWindow("depth",0); cv::imshow("depth",cv::Mat1d(img)*0.001); cv::waitKey(100); });
	
	while (true) { sleep(1);}
}
