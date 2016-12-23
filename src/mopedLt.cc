////////////////////////////////////////////////////////////////////////
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs` -lboost_system -pthread -lturbojpeg  -lboost_system -lboost_program_options -O2 -Wno-inline

#include <opencv2/opencv.hpp>
#include <boost/program_options.hpp>
#include <uSnippets/log.hpp>
#include <dirent.h>


using namespace uSnippets;


struct HeatMap {
	
	cv::Mat1f H;
	
	HeatMap() {}
	
	HeatMap(cv::Size size) : H(size,0.f) {}
	
	HeatMap &filter(double cooling=0.90, double sigma=3) {
		
		H *= cooling;
		cv::GaussianBlur(H,H,cv::Size(),sigma,sigma);
		return *this;
	}
	
	
	
	HeatMap &heat(cv::Point2f pt, double temperature) {
/*
		cv::Mat1f row(1,H.cols,0.f);
		for (int j=0; j<H.cols; j++)
			row(j) = std::exp(- std::pow((j-pt.x)/2,2)/(2*sigma*sigma) );

		cv::Mat1f col(H.rows,1,0.f);
		for (int i=0; i<H.rows; i++)
			col(i) = temperature*std::exp(- std::pow((i-pt.y)/2,2)/(2*sigma*sigma) );
		
		for (int i=0; i<H.rows; i++)
			for (int j=0; j<H.cols; j++)
				H(i,j) += col(i)*row(j);*/
		if (pt.x>=H.cols or pt.y>=H.rows) return *this;
		H(pt)+=temperature;
		return *this;
	}
	
	cv::Mat3b draw(cv::Mat3b img, cv::Vec3b color, float scale=0) const {
			
		cv::Mat3b out = img.clone();
		Assert(img.size==H.size) << "Heat map not the same size as the image";

		if (scale==0) {
			for (int i=0; i<H.rows; i++)
				for (int j=0; j<H.cols; j++)
					scale = std::max(scale,H(i,j));
			scale = 1./(scale+1e-10);
		}

		for (int i=0; i<H.rows; i++) {
			for (int j=0; j<H.cols; j++) {
				double h = std::min(std::max(H(i,j)*scale,0.f),1.f);
				out(i,j)/= 1+h;
				out(i,j) += h*color;
			}
		}				
		return out;
	}
	
	operator bool() const { return not H.empty(); }
};

namespace Geometry {
	
	typedef cv::Vec4f Vec4; // third component should always be 1
	typedef cv::Matx44f Mat44;

/*	struct PinHole { double fx, fy, cx, cy; };
	
	class Pose {
		mutable Mat44 transformMatrix, iTransformMatrix;
	public:

		const Vec4   translation;
		const Vec4   quaternion;
		
		Pose( const Vec4 &translation, const Vec4 &rotation ): translation(translation), rotation(rotation), transformMatrix(Mat44::zeros()), iTransformMatrix(Mat44::zeros()) {};

		const Mat44 &getTransformMatrix() const {
			
			const auto &q = quaternion;
			if (transformMatrix(4,4)==0) transformMatrix = Mat44(
				q[3]*q[3]+q[0]*q[0]-q[1]*q[1]-q[2]*q[2],					2*q[0]*q[1]+2*q[3]*q[2],					2*q[0]*q[2]-2*q[3]*q[1], 	translation[0],
								2*q[0]*q[1]-2*q[3]*q[2], 	q[3]*q[3]-q[0]*q[0]+q[1]*q[1]-q[2]*q[2],				 	2*q[1]*q[2]+2*q[3]*q[0], 	translation[1],
								2*q[0]*q[2]+2*q[3]*q[1], 					2*q[1]*q[2]-2*q[3]*q[0], 	q[3]*q[3]-q[0]*q[0]-q[1]*q[1]+q[2]*q[2], 	translation[2],
													0.f, 										0.f,										0.f, 	  			1.);
			return transformMatrix;
		}

		const Mat44 &getInvTransformMatrix(const Vec4 &t, const Vec4 &q) const {
			
			getTransformMatrix();
			const auto &tm = transformMatrix;
			if (iTransformMatrix(4,4)==0) iTransformMatrix = Mat44(
				tm(0,0), tm(1,0), tm(2,0), -(tm(0,0)*tm(0,3) + tm(1,0)*tm(1,3) + tm(2,0)*tm(2,3)),
				tm(0,1), tm(1,1), tm(2,1), -(tm(0,1)*tm(0,3) + tm(1,1)*tm(1,3) + tm(2,1)*tm(2,3)),
				tm(0,2), tm(1,2), tm(2,2), -(tm(0,2)*tm(0,3) + tm(1,2)*tm(1,3) + tm(2,2)*tm(2,3)),
					  0,	   0, 	    0,													 	1);
			return iTransformMatrix;
		}

		bool operator< (const Pose &p) const { return translation=p.translation?rotation<p.rotation:translation<p.translation; }

		friend std::ostream& operator<< (std::ostream &out, const Pose &p) { out << pt.translation << " " << pt.rotation; return out; }
	};

	// Transform a point in Object Coordinates to World Coordinates
	inline Vec4 tObjectToWorld( const Vec4 &p3D, const Pose &objectPose ) { return objectPose.getTransformMatrix()*p3D; }

	// Transform a point in World Coordinates to Camera Coordinates
	inline Vec4 tWorldToCamera( const Vec4 &p3D, const Pose &camPose    ) { return camPose.getInvTransformMatrix()*p3D; }

	// Transform a point in Object Coordinates to Camera Coordinates
	//static inline Vec4 tWorldToCamera( const Vec4 &p3D, const Pose &objectPose, const Pose &camPose ) { return camPose.iTransformMatrix*p3D; }
	
	inline cv::Vec2f projectToFrame( const Vec4 &p3D, const PinHole &pinhole) { 
		return { proj3D[0]/(proj3D[2]+std::numeric_limits<float>::min()) * pinhole.fx + pinhole.cx, 
				 proj3D[1]/(proj3D[2]+std::numeric_limits<float>::min()) * pinhole.fy + pinhole.cy };
	}*/
}

struct ORBFeatureEngine {
	
	cv::ORB orb = cv::ORB(2000,1.15f,12);

	struct Descriptor {
		
		const cv::Mat1b d;
		
		Descriptor(const cv::Mat1b &d) : d(d.clone()) {}
		
		uint dist(const Descriptor &desc) const {
			const uint64_t *v0 = reinterpret_cast<const uint64_t*>(d.data);
			const uint64_t *v1 = reinterpret_cast<const uint64_t*>(desc.d.data);
			return _popcnt64(v0[0]^v1[0])+_popcnt64(v0[1]^v1[1])+_popcnt64(v0[2]^v1[2])+_popcnt64(v0[2]^v1[2]);
		}
	};
	
	std::vector<std::pair<cv::KeyPoint,Descriptor>> extract(cv::Mat3b img) {
		
		std::vector<std::pair<cv::KeyPoint,Descriptor>> desc;
		
		std::vector<cv::KeyPoint> keyPoints;
		cv::Mat1b descriptors;
		orb(img, cv::Mat(), keyPoints, descriptors);
		for (size_t i=0; i<keyPoints.size(); i++)
			desc.emplace_back(keyPoints[i], descriptors.row(i));

		return desc;
	}		
	
};

/*
struct SURFFeatureEngine {
	
	cv::SURF surf(500);

	struct Descriptor {
		
		const cv::Mat1f d;
		
		Descriptor(const cv::Mat1f &d) : d(d.clone()) {}
		
		float dist(const Descriptor &desc) const {
			return cv::norm(d-desc.d);
		}
	};
	
	std::vector<std::pair<cv::KeyPoint,Descriptor>> extract(cv::Mat3b img) {
		
		std::vector<std::pair<cv::KeyPoint,Descriptor>> desc;
		
		std::vector<cv::KeyPoint> keyPoints;
		cv::Mat1f descriptors;
		surf(img, cv::Mat(), keyPoints, descriptors);
		for (size_t i=0; i<keyPoints.size(); i++)
			desc.emplace_back(keyPoints[i], descriptors.row(i));

		return desc;
	}		
	
};*/

template<typename FeatureEngine>
struct MopedLt {

	static inline std::vector<std::string> getAllFilenames(std::string path, std::string type="") {
		
		std::vector<std::string> r;
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir (path.c_str())) == NULL)
			return r;
		
		while ((ent = readdir (dir)) != NULL)
			if (std::string(ent->d_name).size()>=type.size() and std::string(&ent->d_name[std::string(ent->d_name).size()-type.size()])==type)
				r.push_back(path+"/"+ent->d_name);

		closedir (dir);
		std::sort(r.begin(), r.end());
		return r;
	}

	FeatureEngine FE;
	typedef typename FeatureEngine::Descriptor Descriptor;
	
	struct ImageKeyPoint {
		
		cv::KeyPoint kp;
		Descriptor desc;
		cv::Mat3b img;
		ImageKeyPoint(const cv::KeyPoint &kp, const Descriptor &desc, const cv::Mat3b &img) : kp(kp), desc(desc), img(img) {}
	};

	struct ModelKeyPoint {
		
		Geometry::Vec4 pt;
		std::vector<ImageKeyPoint> ikps;
		ModelKeyPoint(const cv::KeyPoint &kp, const Descriptor &desc, const cv::Mat3b &img) { ikps.emplace_back(kp,desc,img); }
	};
	
	typedef std::vector<ModelKeyPoint> Model;

	std::vector<std::pair<std::string, Model>> models;

	MopedLt &add(std::string path) {
		
		for (auto &model : getAllFilenames(path)) {
			
			std::vector<cv::Mat3b> images;
			for (auto &imageName : getAllFilenames(model)) {
				cv::Mat3b image = cv::imread(imageName);
				while (image.cols>1024) cv::resize(image,image,cv::Size(),.5,.5);
				if (not image.empty())
					images.emplace_back(image);
			}
			if (images.empty()) continue;
			
			std::string name = model.substr(model.find_last_of("/\\")+1);
			Log(0) << name << ": " << images.size();
			
			add(name, images);
		}
		return *this;
	}

	MopedLt &add(std::string name, std::vector<cv::Mat3b> images) {
		
		models.emplace_back(name, Model());

		Model &model = models.back().second;
		
		Assert(images.size()>2) << "Not enough images for SFM";
		
		// We assume images are correlated, therefore an almost-planar transformation must exist between consecutive images
		Log(0) << "KeyPoint Extraction";
		{
			for (auto &p : FE.extract(images[0]))
				model.emplace_back(p.first, p.second, images[0]);
			
			for (size_t ii = 1; ii<images.size(); ii++) {
				Log(0) << "Image " << ii << " of " << images.size();

				auto feats = FE.extract(images[ii]);
				Assert(feats.size()>4) << "Not enough features in image" << ii;
				
				std::vector<size_t> bestMatchA(model.size()), bestMatchB(feats.size());
				for (size_t i=0; i<model.size(); i++) {
					for (size_t j=0; j<feats.size(); j++) {
						
						if (model[i].ikps.back().desc.dist(feats[j].second) < model[i].ikps.back().desc.dist(feats[bestMatchA[i]].second))
							bestMatchA[i] = j;

						if (model[i].ikps.back().desc.dist(feats[j].second) < model[bestMatchB[j]].ikps.back().desc.dist(feats[j].second))
							bestMatchB[j] = i;
					}
				}
				
				std::vector<cv::Point2f> pointsA, pointsB;
				for (size_t j=0; j<feats.size(); j++) {
					if (bestMatchA[bestMatchB[j]]==j) {
						pointsA.push_back(model[bestMatchB[j]].ikps.back().kp.pt);
						pointsB.push_back(feats[j].first.pt);
					}
				}
				
				cv::Mat maskG;
				cv::Mat H = findHomography( pointsA, pointsB, cv::RANSAC, 5, maskG );
				cv::Mat1b mask = maskG;
				if (H.empty()) Log(0) << "Homography found: " << H.empty();
				
				{

					std::vector<cv::KeyPoint> kp;
					for (size_t j=0,k=0; j<feats.size(); j++) {
						if (bestMatchA[bestMatchB[j]]!=j) continue;
						if (mask(k)) 
							kp.push_back(feats[j].first);
						k++;
					}

					cv::Mat3b displayImg = images[ii].clone();
					cv::drawKeypoints(displayImg, kp, displayImg);
					cv::imshow("kk",displayImg);
					cv::waitKey(10);
				}
				
				for (size_t j=0,k=0; j<feats.size(); j++) {
					if (bestMatchA[bestMatchB[j]]!=j) continue;
					if (mask(k)) {
						model[bestMatchB[j]].ikps.push_back({feats[j].first, feats[j].second, images[ii]});
					} else {
						model.emplace_back(feats[j].first, feats[j].second, images[ii]);
					}
					k++;
				}
			}
		}
		Log(0) << "KeyPoint Extraction Done: " <<model.size() << " current points";
		
		Log(0) << "Removing low cardinality points";
		{
			std::vector<ModelKeyPoint> modelTmp;
			std::swap(model, modelTmp);
			for (auto &m : modelTmp)
				if (m.ikps.size()>2)
					model.push_back(m);
		}
		Log(0) << "Removing low cardinality points: Done, " <<model.size() << " current points";
		return *this;
	}
	
//	Point *query(const Descriptor &) {}

//	std::vector<std::pair<cv::Point2f,Point *>> queryN(cv::Mat3b img) {
//	}

	std::string queryLargestObject(cv::Mat3b image, HeatMap &heatMap) {
		
		std::vector<cv::KeyPoint> kps;
		std::map<std::pair<std::string,Model>*,double> mp;
		
		for (auto &f : FE.extract(image)) {
			
			struct MatchInfo { double distance; std::pair<std::string,Model> *m; ModelKeyPoint *k; };
			MatchInfo M[2]; M[0].distance = M[1].distance = 1e10;
			for (auto &m : models) {
				for (auto &k : m.second) {
					for (auto &ikp : k.ikps) {
						double d = ikp.desc.dist(f.second);
						if (d>=M[1].distance) continue;
						M[1].distance = d;
						M[1].m = &m;
						M[1].k = &k;
						if (M[0].distance > M[1].distance) std::swap(M[0],M[1]);
					}
				}
			}
			Log(-2) << M[0].distance << " " << M[1].distance;
			if (M[0].k != M[1].k) continue;
			//if (M[0].distance > 40) continue; //Magic number!			
			kps.push_back(f.first);
			mp[M[0].m] += std::pow(1/M[0].distance,2);

			if (heatMap)
				heatMap.heat(f.first.pt,.2);
		}
		heatMap.filter();
		
		Log(0) << kps.size();
		if (0) {
			cv::Mat3b displayImg = image.clone();
			cv::drawKeypoints(displayImg, kps, displayImg);
			cv::imshow("kk",displayImg);
			cv::waitKey(10);
		}
		
		std::pair<std::string,Model> *best=&models.front();
		for (auto &m : mp)
			if (m.second>mp[best])
				best = m.first;
		
		for (auto &m : mp)
			Log(0) << m.first->first << ": " << m.second; 
		
		return "";
	}
};

/*
class MonoSFM {




public:
	
	std::map<std::string, std::vector<cv::KeyPoint>> imageKeypoints;
	std::map<std::string, cv::Mat1b> imageDescriptors;
	std::map<std::string, cv::Mat1f> imageFundamentals;
	
	cv::Mat3b panorama;
	std::vector<cv::KeyPoint> panoramaKeypoints;
	cv::Mat1b panoramaDescriptors;
	
	MonoSFM(const std::Mat3b &pattern, double ppm, const std::vector<std::string> &imageFiles) {
		
		
		std::vector<cv::KeyPoint> lastKeypoints;
		cv::Mat1b lastDescriptors;
	
		orb(pattern, cv::Mat(), lastKeypoints, lastDescriptors);
		
		Log(0) << "Extracting all descriptors";
		for (auto &imageFile : imageFiles) {
			
			cv::Mat3b image = cv::imread(imageFile);

			std::vector<cv::KeyPoint> keypoints;
			cv::Mat1b descriptors;
			orb(image, cv::Mat(), keypoints, descriptors);
			
			if (descriptors.rows<8) throw(std::string("Not enough keypoints"));
			
			for (int i=0; i<descriptors.rows; i++) {
				std::vector<std::pair<int, uint>> dists;
				for (int j=0; j<lastDescriptors.rows; j++)
					dists.emplace_back(j,dist(descriptors.row(i), lastDescriptors.row(j)));
				
				std::partial_sort(dists.begin(), dists.begin()+3, dists.end());
				
				Log(0) << dists[0].second << " " << dists[1].second; 
			}
			
			lastKeypoints = keypoints;
			lastDescriptors = descriptors;
		}		
	}
};*/


int main(int argc, char *argv[]) {
		
	// PARSING PROGRAM OPTIONS
	namespace po = boost::program_options;
	po::options_description pod("BlindSLAM Tool");

	po::positional_options_description p;
	p.add("path", 1);

	pod.add_options() 
		("help,h", "produce this help message")
		("log,l", po::value<int>()->default_value(0), "set log level")
		("path", po::value<std::string>()->default_value("data/objects"), "path to SFM files");
	
	po::variables_map pom;
	po::store( po::command_line_parser( argc, argv).options(pod).positional(p).run(), pom);
	po::notify(pom);

	if (pom.count("help")) {
		std::cout << "Usage:" << std::endl <<  pod << "\n";
		return 0;
	}
	
	Log::reportLevel(pom["log"].as<int>());

	MopedLt<ORBFeatureEngine> moped;
	moped.add(pom["path"].as<std::string>());
	
	cv::VideoCapture cap(0);
    if (cap.isOpened()) {
		while (cv::waitKey(10)!='q') {
			cv::Mat3b image;
			cap >> image;
			HeatMap heatMap(image.size());
			moped.queryLargestObject(image,heatMap);
			cv::imshow("kk",heatMap.draw(image,{0,0,255}));
		}		
	} else {
		for (auto &model : moped.getAllFilenames(pom["path"].as<std::string>())) {
				
			for (auto &imageName : moped.getAllFilenames(model)) {
				cv::Mat3b image = cv::imread(imageName);
				while (image.cols>1024) cv::resize(image,image,cv::Size(),.5,.5);
				if (image.empty()) continue;
				HeatMap heatMap(image.size());
				moped.queryLargestObject(image,heatMap);
				cv::imshow("kk",heatMap.draw(image,{0,0,255}));
				cv::waitKey(10);			
			}
		}
	}
}
