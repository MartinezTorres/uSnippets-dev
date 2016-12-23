////////////////////////////////////////////////////////////////////////
// HADAMARD encoder
//
// Manuel Martinez (salutte@gmail.com)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs`

#include <uSnippets/hadamard.hpp>

#include <opencv/highgui.h>
#include <iostream>

using namespace std;
using namespace uSnippets;

int main(int argc, char *argv[]) {
	
	if (argc<3) {
		cout << "Use: ./hadamard <file.png> <Q>" << endl;
		cout << "E.g. ./hadamard lena.png 50" << endl;
		return -1;
	}

	cv::Mat1b img = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	//cv::GaussianBlur(img, img, cv::Size(), .5);
	
	cv::Mat1b img_t = img.clone();
	string encoded = Hadamard::encode(img_t, atoi(argv[2]));
	
	cv::Mat1b img2 = Hadamard::decode(encoded);
	
	
	cv::Mat1b img22, img44, imgt22, imgt44;
	cv::resize(img, img22, cv::Size(), .5, .5, cv::INTER_AREA);
	cv::resize(img, img44, cv::Size(), .25, .25, cv::INTER_AREA);
	cv::resize(img_t, imgt22, cv::Size(), .5, .5, cv::INTER_AREA);
	cv::resize(img_t, imgt44, cv::Size(), .25, .25, cv::INTER_AREA);
	cout << cv::sum(cv::abs(img44-imgt44))[0]/(img44.rows*img44.cols) << endl;
	cout << cv::sum(cv::abs(img22-imgt22))[0]/(img22.rows*img22.cols) << endl;
	cout << cv::sum(cv::abs(img-img_t))[0]/(img.rows*img.cols) << endl;

	cv::imshow("orig", img);
	cv::imshow("decoded", img_t);
	cv::waitKey(0);
	return 0;
}

// Thanks for R.T.F.C.
