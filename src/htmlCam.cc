////////////////////////////////////////////////////////////////////////
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs` -lboost_system -pthread -lturbojpeg -O2 -Wno-inline

#include <uSnippets/gui2dweb.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

int main() {

	uSnippets::Gui2Dweb gui("htmlCam", 8080, [](uSnippets::Gui2Dweb::Gui2Dm &gui) {
		
		gui.camCB = [&](cv::Mat3b img){gui.img(img);};
		
		while(true) sleep(1);
		/*cv::VideoCapture cap(0);
		cv::Mat3b img;
		while (true) {
			cap >> img;
			gui.img(img);
		}*/
	});
	
	while(true) sleep(1);
}
