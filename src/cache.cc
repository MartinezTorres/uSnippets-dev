////////////////////////////////////////////////////////////////////////
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs`

#include <uSnippets/cache.hpp>
#include <opencv/cv.h>
#include <iostream>

using namespace std;
using namespace uSnippets;

int main() {
/*	
	{
		cout << Serializer::serialize(std::string("Hi Folkks!"))  << endl;
//		cout << Serializer::serialize("Hi Folkks!") << endl;
	}
	{
		//cout << Serializer::serialize(*(initializer_list<int> *)nullptr) << endl;
	}
	{
		cout << Serializer::serialize(std::vector<cv::Mat>(10,cv::Mat1b(3,3,'0'))) << endl;
	}
	if(0){
		std::vector<cv::Mat1b> vec(1000000,cv::Mat1b(128,16,'0'));
		cout << "Serializing!" << endl;
		std::string s = Serializer::serialize(vec);
		cout << "Done!" << endl;
		vec.clear();
		cout << "Unserializing!" << endl;
		vec = Serializer::unserialize<decltype(vec)>(s);
		cout << "Done!" << endl;
	}
	{
		GenericCache cache("/tmp/test.cache");
		cout << "Testing! " << cache.getKeys().size() << endl;
		for (int i=0; i<4; i++) {
			cache.purge();
			cout << "Write: " << i << endl;
			for (int k=0; k<10000; k++)
				cache.set(object("test")+rand()%10000, std::string(rand()%10,'#'));
			cout << "Read: " << i << endl;	
			std::string str;
			for (int k=0; k<10000; k++)
				 cache.get(object("test")+rand()%10000, str);
		}
//		cache.writeIndex();
		cout << "Done!" << endl;
	}
	{
		GenericCache cache("/tmp/test.cache");
		cout << "Test indexing! " << cache.getKeys().size() << endl;
	}
	{
		GenericCache cache("/tmp/test.cache");
		cout << "Test indexing! " << cache.getKeys().size() << endl;
	}*/
}
