////////////////////////////////////////////////////////////////////////
// ENCODING IMAGES USING AVCONV or FFMPEG 
//
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs` -lavcodec -lavutil -lboost_system -lboost_program_options

#include <uSnippets/libav.hpp>
#include <fstream>
#include <opencv/highgui.h>
#include <boost/program_options.hpp>

using namespace std;
using namespace uSnippets;

static inline double getPSNR(const cv::Mat& I1, const cv::Mat& I2) {
	
    cv::Mat s1;
    cv::absdiff(I1, I2, s1);       // |I1 - I2|
    s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
    s1 = s1.mul(s1);           // |I1 - I2|^2

    cv::Scalar s = cv::sum(s1);        // sum elements per channel

    double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

    if (sse <= 1e-10) // for small values return zero
        return 0;

	double mse  = sse / (double)(I1.channels() * I1.total());
	double psnr = 10.0 * log10((255 * 255) / mse);
	return psnr;
}

struct WAVE {

	string fmt;
	
	vector<LibAV::MP2Pkg> read(string file) {
		
		vector<LibAV::MP2Pkg> out;
		
		char descriptor[5] = {0,0,0,0,0};
		uint32_t size;
		ifstream wav(file);
		wav.read(descriptor,4);
		wav.read((char *)&size,4);
		wav.read(descriptor,4);
		while (true) {
			wav.read(descriptor,4);
			if (!wav) break;
			wav.read((char *)&size,4);
			if (string(descriptor)=="fmt ") {
				fmt.resize(size);
				wav.read(&fmt[0],size);
			} else if (string(descriptor)=="data") {
				out.resize(size/sizeof(LibAV::MP2Pkg));
				for (uint i=0; i<out.size(); i++)
					wav.read((char *)&out[i],sizeof(LibAV::MP2Pkg));
			} else {
				wav.seekg(size,wav.cur);
			}
		}
		return out;
	}

	void write(string file, const vector<LibAV::MP2Pkg> data) {
		
		ofstream wav(file);
		wav.write("RIFF", 4);
		uint32_t size = 4+4+4+4+16+4+data.size()*sizeof(LibAV::MP2Pkg); wav.write((char *)&size, 4);
		wav.write("WAVE", 4);
		wav.write("fmt ", 4);
		size = 16; wav.write((char *)&size, 4);
		wav.write(&fmt[0],16);
		wav.write("data",4);
		size = data.size()*sizeof(LibAV::MP2Pkg); wav.write((char *)&size, 4);
		for (uint i=0; i<data.size(); i++)
			wav.write((char *)&data[i],sizeof(LibAV::MP2Pkg));
	}
};

int main(int argc, char **argv) {
	
	// PARSING PROGRAM OPTIONS
	namespace po = boost::program_options;
	po::options_description global("Options for the libav test file");
	global.add_options() 
		("help,h", "produce this help message")
		("command", po::value<std::string>(), "command to execute, can be audio, video or ffv1")
		("subargs", po::value<std::vector<std::string> >(), "Arguments for command");

	po::positional_options_description pos;
	pos.add("command", 1).add("subargs",-1);

	po::parsed_options parsed = po::command_line_parser(argc, argv).options(global).positional(pos).allow_unregistered().run();
	po::variables_map vm;
	po::store(parsed, vm);
	
	if (vm.count("help") or not vm.count("command")) {
		cout << "Usage: libav {audio|video} options" << endl;
		return 0;
	}

	// Collect all the unrecognized options from the first pass. This will include the
	// (positional) command name, so we need to erase that.
	std::vector<std::string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
	opts.erase(opts.begin());

	std::string cmd = vm["command"].as<std::string>();
	if (cmd == "audio") {

		po::options_description pod("audio options");
		pod.add_options()
			("file", po::value<std::string>(), "RAW 16bit stereo 44100 audio file");

		po::positional_options_description pos;
		pos.add("file", 1);

		po::store(po::command_line_parser(opts).options(pod).positional(pos).run(), vm);
		
		if (vm.count("file")) {
			WAVE wave;
			
			int bitrate = 192000;
			vector<LibAV::MP2Pkg> src = wave.read(vm["file"].as<std::string>());
			cout << "Compressing " << src.size() << " pkgs with bitrate " << bitrate << endl;
			string compressed = LibAV::mp2encode(src,bitrate);
			cout << "Compressed to size: " << compressed.size()/(1e-10+src.size()*sizeof(LibAV::MP2Pkg)) << endl;
			vector<LibAV::MP2Pkg> out = LibAV::mp2decode(compressed);
			cout << "Decompressed to npkg: " << out.size() << endl << endl;
			wave.write("out.wave",out);
			
		} else {
			cout << "Synthetic audio testing" << endl;
			
			int bitrate = 128000;
			for (int i=1; i<10; i++) {
				LibAV::MP2Pkg pkg;
				for (uint j=0; j<pkg.size(); j++) {
					pkg[j][0] = 1024*sin(j*3.14*2*440/44100);
					pkg[j][1] = 1024*sin(j*3.14*2*440/44100+2);
				}
				vector<LibAV::MP2Pkg> src(i,pkg);
				cout << "Compressing " << i << " pkgs with bitrate " << bitrate << endl;
				string compressed = LibAV::mp2encode(src,bitrate);
				cout << "Compressed to size: " << compressed.size()/(1e-10+i*sizeof(LibAV::MP2Pkg)) << endl;
				vector<LibAV::MP2Pkg> out = LibAV::mp2decode(compressed);
				cout << "Decompressed to npkg: " << out.size() << endl << endl;
			}
		}
	
	} else if (cmd == "video") {

		po::options_description pod("audio options");
		pod.add_options()
			("file,f", po::value<std::string>(), "RAW 16bit stereo 44100 audio file");

		po::positional_options_description pos;
		pos.add("file", 1);

		po::store(po::command_line_parser(opts).options(pod).positional(pos).run(), vm);
		
		shared_ptr<cv::VideoCapture> imgsrc;
		if (vm.count("file"))
			imgsrc = make_shared<cv::VideoCapture>(vm["file"].as<std::string>());
		else
			imgsrc = make_shared<cv::VideoCapture>(0);
		
		if (not imgsrc->isOpened()) exit(-1);

		vector<LibAV::ImgYUV480P> in, out;
		while ((cv::waitKey(10)&255) != 'q') {
			
			cv::Mat3b img;
			*imgsrc >> img;
			in.emplace_back(img);
			
			if (in.size()==200) {
				
				cout << "Coding " << in.size()*img.rows*img.cols*img.elemSize() << " bytes" << endl;
				std::string coded = LibAV::h264encode(in, "1", "ultrafast");
				cout << "Coded to " << coded.size() << " bytes  1-" << (in.size()*img.rows*img.cols*img.elemSize())/(1.*coded.size()) << " ratio" << endl;
				out = LibAV::h264decode(coded);
				cout << "Decoded to " << out.size() << " images " << endl;

				for (uint i=0; i<out.size(); i++) {
					
					cout << getPSNR(in[i].bgr(), out[i].bgr()) << endl;
					cv::imshow("cam1", out[i].bgr());
					cv::imshow("cam2", in[i].gray());
					cv::Mat1b cmp; cv::compare(in[i].gray(), out[i].gray(), cmp, cv::CMP_NE);
					cv::imshow("cam3", cmp);
					cv::waitKey(50);
				}
				
				in.clear();
			}		
		}
		
	} else if (cmd == "ffv1") {
/*
		
		vector<cv::Mat1s> in, out;
		for (int i=0; i<15; i++) {
			in.push_back(cv::Mat1s(480,640));
			for (auto &v : in.back())
				v = rand()&0x1FFF;
		}
		
		cout << "Coding " << in.size()*in.front().rows*in.front().cols*in.front().elemSize() << " bytes" << endl;
		std::string coded = LibAV::ffv1encode(in);
		cout << "Coded to " << coded.size() << " bytes  1-" << (in.size()*in.front().rows*in.front().cols*in.front().elemSize())/(1.*coded.size()) << " ratio" << endl;
		out = LibAV::ffv1decode<int16_t>(coded);
		cout << "Decoded to " << out.size() << " images " << endl;
		cout << "Is equal: " << (in.size() == out.size()) << endl;*/
		
	} else {
		cout << "Usage: libav {audio|video} options" << endl;
		return 0;
	}
}
