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
//	cv::namedWindow("depth",0);
//	openni.colorCB([](cv::Mat3b img){cv::imshow("color",img);});
	
	deque<cv::Mat1s> depths;
	openni.depthCB([&](cv::Mat1s img){depths.push_back(img.clone()); });
//	cv::namedWindow("test", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_NORMAL);
/*
	while ((cv::waitKey(50) & 255) != 27) {
		
		cv::Mat1b testIM(rows,cols,uint8_t(0));
		for (int i=0; i<rows; i++) {
			for (int j=0; j<cols; j++) {
				std::map<int, int> s; 
				for (uint k=0; k<dp.size(); k++) {
					s[dp[k](i,j)]++;
				}
				testIM(i,j) = min(255,int(s.size()-1)*64);
			}
		}
		
		cv::imshow("test", testIM);
		
		//std::string out2;
		//MRD::Codec::GZip::code(out, out2,4);
		//cout << out2.size()/1024 << " " << out2.size()/float(rows*cols*2*dp.size()) << endl;
	}*/
}
