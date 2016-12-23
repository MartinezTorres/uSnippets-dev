////////////////////////////////////////////////////////////////////////
// Small utility on testing swap performance for image workloads
//
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs` -lboost_system -lboost_program_options -O2

#include <opencv2/opencv.hpp>
#include <boost/program_options.hpp>

#include <uSnippets/log.hpp>
using namespace std;
using uSnippets::Log;

int main(int argc, char *argv[]) {
		
	// PARSING PROGRAM OPTIONS
	namespace po = boost::program_options;
	po::options_description pod("SWAPTest Tool");

	po::positional_options_description p;
	p.add("size", 1);

	pod.add_options() 
		("help,h", "produce this help message")
		("log,l", po::value<int>()->default_value(0), "set log level")
		("size", po::value<double>()->default_value(1024), "memory size to test in MB");
	
	po::variables_map pom;
	po::store( po::command_line_parser( argc, argv).options(pod).positional(p).run(), pom);
	po::notify(pom);

	if (pom.count("help")) {
		cout << "Usage:" << endl <<  pod << "\n";
		return 0;
	}
	
	Log::reportLevel(pom["log"].as<int>());
	
	double usedMemory = 0;
	std::vector<cv::Mat3b> images;
	Log(0) << "Start Creating images";
	while (usedMemory < pom["size"].as<double>()*(1<<20)) {
		
		cv::Mat3b img(224,244); 
		img(0,0)=1;
		images.push_back(img.clone());
		
		usedMemory += 224*224*3;	
	}
	Log(0) << "Images Created";
	
	for (int i=0; i<10; i++) {
		
		Log(0) << "Iteration " << i;
		
		double norm = 0;
		for (int j=0; j<10000; j++) {
			norm += cv::norm(images[rand()%images.size()]);
		}
		
		Log(0) << "Finished, and norm was: " << norm;
	}
}
