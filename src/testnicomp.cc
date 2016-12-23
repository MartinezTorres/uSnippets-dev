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

#include <uSnippets/range.hpp>

using namespace std;
using namespace uSnippets;

static inline void score(const vector<uint16_t> &data) {

	int nz = 0, nnz = 0, n1 = 0;
	int lastsign=1;
	uint16_t last=0;	
	for (auto &i : data) {
		
		if (i-last)
			nnz++;
		else 
			nz++;

		if (i-last == -lastsign) n1++;

		if (i-last>0) lastsign = 1;
		if (i-last<0) lastsign = -1;

			
		last = i;
	}
	cout << "Nzero: " << double(100*nz)/data.size() << endl;	
	cout << "None: " << double(100*n1)/data.size() << endl;	

	cout << "Nfast: " << double(100*(nz+n1))/data.size() << endl;	
	
}

static inline string compress(const vector<uint16_t> &data) {
	
	vector<uint8_t> zeros, ones, signs;
	vector<uint16_t> outdata;
	
	int lastsign=1;
	uint16_t last=0;	
	int maxOutData = 0;
	for (auto &i : data) {
		int c = i-last;
		last = i;
		
		zeros.push_back(!!c);
		if (c) {
			ones.push_back(c==lastsign);
			
			if (c!=lastsign) {
				signs.push_back(c>0);
				outdata.push_back(abs(c));
				maxOutData = max(maxOutData,abs(c));
			}
			if (c<0) lastsign = 1;
			if (c>0) lastsign = -1;
		}
	}
	cout << "Vectors prepared" << endl;
/*	cout << "ZerosBin(0) compressed to:" << uSnippets::Codecs::Range::encode(zeros,2,12).size() << endl;
	cout << "ZerosBin(0) compressed to:" << uSnippets::Codecs::Range::encodeBin(zeros,5).size() << endl;
	cout << "ZerosBin(0) compressed to:" << uSnippets::Codecs::Range::encodeBin(zeros,10).size() << endl;
	cout << "ZerosBin(0) compressed to:" << uSnippets::Codecs::Range::encodeBin(zeros,20).size() << endl;
	cout << "onesBin(12) compressed to:" << uSnippets::Codecs::Range::encodeBin(ones,5).size() << endl;
	cout << "signs(12) compressed to:" << uSnippets::Codecs::Range::encodeBin(signs,5).size() << endl;
	cout << "outdata(3) compressed to:" << uSnippets::Codecs::Range::encode(outdata,maxOutData+1,3).size() << endl;
	cout << "outdata(3) compressed to:" << uSnippets::Codecs::Range::encode(outdata,maxOutData+1,6).size() << endl;
	cout << "outdata(3) compressed to:" << uSnippets::Codecs::Range::encode(outdata,2048,0).size() << endl;
	cout << "outdata(3) compressed to:" << uSnippets::Codecs::Range::encode(outdata,2048,3).size() << endl;
	cout << "outdata(3) compressed to:" << uSnippets::Codecs::Range::encode(outdata,2048,6).size() << endl;*/
	cout << "Start compression" << endl;
	string comp; 
	comp += uSnippets::Codecs::Range::encodeBin(zeros,10);
	comp += uSnippets::Codecs::Range::encodeBin(ones,10);
	comp += uSnippets::Codecs::Range::encodeBin(signs,10);
	comp += uSnippets::Codecs::Range::encode(outdata,2048,3);
	cout << "End compression: " << comp.size()/1024 << "KBi" << endl;
/*	istringstream is(comp);
	cout << "check zero: " <<(zeros==uSnippets::Codecs::Range::decodeBin(is)) << endl; 
	cout << "check ones: " <<(ones ==uSnippets::Codecs::Range::decodeBin(is)) << endl; 
	cout << "check sign: " <<(signs==uSnippets::Codecs::Range::decodeBin(is)) << endl; 
	cout << "check data: " <<(outdata==uSnippets::Codecs::Range::decode(is,2048)) << endl; */

	return comp;
}

static inline vector<uint16_t> uncompress(const string &in) {
	
	istringstream is(in);
	vector<uint8_t> zeros = uSnippets::Codecs::Range::decodeBin(is);
	vector<uint8_t> ones  = uSnippets::Codecs::Range::decodeBin(is);
	vector<uint8_t> signs = uSnippets::Codecs::Range::decodeBin(is);
	vector<uint16_t> data = uSnippets::Codecs::Range::decode(is,2048);
	uint8_t  *z = &zeros[0], *o = &ones[0], *s = &signs[0];
	uint16_t *d = &data[0];
	
	vector<uint16_t> out(zeros.size());
	
	int lastsign=1;
	uint16_t last=0;	
	for (auto &i : out) {
		
		int c = 0;
		
		if (*z++) {
			
			if (*o++) {
				
				c = lastsign;
			} else {
				c = *d++ * (*s++?1:-1);
			}
			if (c<0) lastsign = 1;
			if (c>0) lastsign = -1;
		}
		
		
		last = i = c + last;
	}
	return out;
}

int main() {
	
	OpenNI openni;

	while (openni) { 
		
		std::chrono::microseconds timeStamp;
		std::vector<cv::Mat1s> depths;
		while (depths.size()<30) depths.push_back(openni.getDepth(timeStamp));
		
		cout << "Start Comp" << endl;
		int rows = depths[0].rows;
		int cols = depths[0].cols;
		
		if(0){
			vector<uint16_t> data;
			for (int i=0; i<rows; i++)
				for (int j=0; j<cols; j++)
					for (uint k=0; k<depths.size(); k++)
						data.push_back(depths[k](i,j)!=2047?depths[k](i,j):0);
			score(data);
		}

		if(0){
			vector<uint16_t> data,data2; data.reserve(rows*cols*30*2);
			for (int i=0; i<rows; i++) {
				for (int j=0; j<cols; j+=2) {
					for (uint k=0; k<depths.size(); k++)
//						data.push_back(depths[k](i,j)!=2047?depths[k](i,j):0);
						data.push_back(depths[k](i,j));

					for (int k=depths.size()-1; k>=0; k--)
//						data.push_back(depths[k](i,j+1)!=2047?depths[k](i,j+1):0);
						data.push_back(depths[k](i,j));
				}		
			}
			
			string c = compress(data);
			data2 = uncompress(c);
			cout << (data2 == data) << endl;
			
		}
		
		{
			
			cout << "Start Compression" << endl;
			string s = Codecs::Disparity::code(depths);
			cout << "Start DeCompression (" << s.size()/1024 << "KBi)" << endl;
			auto depths2 = Codecs::Disparity::decode(s);
			cout << "Done" << endl;
			for (uint i=0; i<depths.size();i++) {
				if (cv::countNonZero(depths[i] != depths2[i])) cout << "Fail!" << endl;
			}
		}
		
		cout << "End Comp" << endl;		
	}
}
