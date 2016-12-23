////////////////////////////////////////////////////////////////////////
// simple GUI code
//
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs` -lSDL -lGLU -lGL -Wno-unused-parameter -Wno-unused-variable


#include <uSnippets/vGUI.hpp>

#include <unistd.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;

int main() {
	
	uSnippets::vGUI::Window vgui;

	cv::VideoCapture cap(0);
	if (not cap.isOpened()) return -1;

//	vgui.widgets3d["top"   ] =  make_shared<vGUI::ImageWidget3d>(cv::Point2f(0.f,0.f));
//	vgui.widgets3d["middle"] =  make_shared<vGUI::ImageWidget3d>(cv::Point2f(1.f,0.f));
//	vgui.widgets3d["bottom"] =  make_shared<vGUI::ImageWidget3d>(cv::Point2f(2.f,7.f));

	while (vgui) {
		
	//	cv::Mat3b img;
	//	cap >> img;
	//	cv::namedWindow("iaiai",CV_GUI_NORMAL | CV_WINDOW_FREERATIO | CV_WINDOW_NORMAL);
	//	if (img.rows and img.cols) cv::imshow("iaiai",img);
		std::this_thread::sleep_for( std::chrono::seconds(1) );
		
//		std::dynamic_pointer_cast<vGUI::ImageWidget3d>(vgui.widgets3d["top"   ])->update(img);
//		std::dynamic_pointer_cast<vGUI::ImageWidget3d>(vgui.widgets3d["middle"])->update(cv::Mat3b(img.t()));
//		std::dynamic_pointer_cast<vGUI::ImageWidget3d>(vgui.widgets3d["bottom"])->update(img);
	}
}
