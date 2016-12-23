////////////////////////////////////////////////////////////////////////
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs` -lboost_system -pthread -lturbojpeg -O2 -Wno-inline

#include <uSnippets/gui2dweb.hpp>
#include <opencv2/opencv.hpp>
#include <uSnippets/log.hpp>

using namespace uSnippets;
//using namespace std;

namespace Geometry {
	
	typedef cv::Vec4f Vec4;
	typedef cv::Matx44f Mat44;

	struct PinHole { double fx, fy, cx, cy; };
	
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

		friend ostream& operator<< (ostream &out, const Pose &p) { out << pt.translation << " " << pt.rotation; return out; }
	};

	// Transform a point in Object Coordinates to World Coordinates
	static inline Vec4 tObjectToWorld( const Vec4 &p3D, const Pose &objectPose ) { return objectPose.getTransformMatrix()*p3D; }

	// Transform a point in World Coordinates to Camera Coordinates
	static inline Vec4 tWorldToCamera( const Vec4 &p3D, const Pose &camPose    ) { return camPose.getInvTransformMatrix()*p3D; }

	// Transform a point in Object Coordinates to Camera Coordinates
	static inline Vec4 tWorldToCamera( const Vec4 &p3D, const Pose &objectPose, const Pose &camPose ) { return camPose.iTransformMatrix*p3D; }
	
	static inline cv::Vec2f projectToFrame( const Vec4 &p3D, const PinHole &pinhole) { 
		return { proj3D[0]/(proj3D[2]+std::numeric_limits<float>::min()) * pinhole.fx + pinhole.cx, 
				 proj3D[1]/(proj3D[2]+std::numeric_limits<float>::min()) * pinhole.fy + pinhole.cy };
	}

}


class MonoSFM {
	

	class Descriptor : public std::array<uint8_t,32> {
		
		using array::array;
		Descriptor(const cv::Mat1b &m) { for (int j=0; j<32; j++) (*this)[j]=m(j); }

		static Descriptor rand() { Descriptor d; for (int j=0; j<32; j++) d[j]=rand()&255; return d; }
		
		uint dist(const Descriptor &d2) {
			
			const uint64_t *v1 = reinterpret_cast<const uint64_t*>(   data());
			const uint64_t *v2 = reinterpret_cast<const uint64_t*>(d2.data());
			
			return _popcnt64(v1[0]^v2[0])+_popcnt64(v1[1]^v2[1])+_popcnt64(v1[2]^v2[2])+_popcnt64(v1[2]^v2[2]);
		}
	}
	
	struct Frame {
		
		cv::Mat3b
		
	};
	
	struct Point {
		
	};
	
	struct DB {
		
		std::vector<std::pair<Descriptor, std::shared_ptr<Point>>> DB;
		
		std::shared_ptr<Point> add(const Descriptor &d, const cv::KeyPoint &kp, std::shared_ptr<Frame> &frame) {
		
		}
		
		DB() {
		}
	};
	

	
	cv::ORB orb;
	
	DB db;

public:

	int maxRANSAC = 100;
	int MinimumKnown = 6;
	int lastPosition = cv::Vec

	void operator()(cv::Mat3b img3b) {
		
		std::vector<cv::KeyPoint> keypoints;
		cv::Mat1b descriptors;
		Log(0) << "Extracting descriptors";
		orb(img3b, cv::Mat(), keypoints, descriptors);
		
		auto sFrame = std::make_shared<Frame>(img3b);
		
		std::vector< std::shared_ptr<Point> > known;
		std::vector< std::shared_ptr<Point> > matches;
		
		for (int i=0; i<descriptors.rows; i++) {
			Descriptor d(descriptors.row(i));
			auto sPoint = DB.add(d,keypoints[i],sFrame);

			if (sPoint.unreliable) continue;
			if (sPoint.known) known.push_back(sPoint);
			if (sPoint.cardinality>1) matches.push_back(sPoint);
		}

		if (known.size()<MinimumKnown) return;
		
		KDTree<std::array<int,2>, std::shared_ptr<Point>> kdtree;

		
		
	}
};

int main() {

	typedef std::unique_lock<std::mutex> Lock;
	std::mutex cvmtx;
	
	uSnippets::Gui2Dweb gui("monoSlam", 8080, [&](uSnippets::Gui2Dweb::Gui2Dm &gui) {
		
		MonoSFM monoSlam;
		std::deque<cv::Mat3b> images;
		gui.camCB = [&](cv::Mat3b img){
			auto l = gui.getLock();
			if (images.size()>1) images.pop_back();
			images.push_back(img);
		};
		
		int timeOut=1000;
		while (timeOut--) {
			std::this_thread::sleep_for(10_ms);	
			
			if (not images.empty()) {
				
				cv::Mat3b img;
				{
					auto l = gui.getLock();
					img = images.front();
					images.pop_front();
				}
				{
					Lock l(cvmtx);
					monoSlam(img);
				}
				gui.img(img);
				timeOut=100;
				cv::waitKey(10);
			}
		}
	});
	while(true) {
		std::this_thread::sleep_for(100_ms);	
	}
}
