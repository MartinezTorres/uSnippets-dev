////////////////////////////////////////////////////////////////////////
// OpenNI capture example
//
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs` ${OPENNI2_REDIST}/libOpenNI2.so -Wno-inline
#include <uSnippets/tensor.hpp>
#include <uSnippets/log.hpp>

#include <type_traits>
#include <typeinfo>
#include <memory>
#include <string>
#include <cstdlib>


using namespace std;
using namespace uSnippets;
//dims={} for i=1,io.read('*n') do dims[i]=io.read('*n') end D=torch.DoubleTensor(unpack(dims)) D:apply(function() return io.read('*n') end) torch.save(io.read():gsub('%s+', ''),D)
//dims={} for i=1,io.read('*n') do dims[i]=io.read('*n') end D=torch.FloatTensor(unpack(dims)) D:apply(function() return io.read('*n') end) print(torch.serialize(D))

#include <fstream>

template <class T>
std::string
type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
           (
#ifndef _MSC_VER
               nullptr,
#else
                nullptr,
#endif
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}


int main() {

//	Assert::verbose(true);
	Log::reportLevel(-2);

	{
		
//		Tensor1f T(2,2);
//		Log(2) << T.exec("cat | sort", "1\n1\n3\n1\n");
		

//		Log(2) << Tensor1f(40*20,5*60,40,26).toTorch().size();
		
		
//		Log(2) << Tensor1f(200,3,4,2,100).rand().nElem();
		//for (auto v : Tensor1f(200,3,4,2,1).rand().clone()) Log(2) << v;
		
//		Log(2) << Tensor1f(20,3,4,2,10).rand().toTorch();
//		Log(2) << Tensor1f(200,3,4,2,100).rand().toTorch().size();
//		Log(2) << Tensor1f(20,3,4,2,100).rand();

		Tensor1f T = Tensor1f(2000,4,5,2).rand();
		Log(0) << T;
		T.view().toTorchHDF5("T.h5", "/T", "w");

//		Tensor1f(20*200,5*60,25,17).toTorchHDF5("test.h5", "/tensor1");
//		Tensor1f(20*200,5*60,25,17).toTorchHDF5("test.h5", "/tensor2");
//		Tensor1f(20*200,5*60,25,17).toTorchHDF5("test.h5", "/tensor3");
//		Tensor1f(20*200,5*60,25,17).toTorchHDF5("test2.h5", "tensor1");
		
//		return 0;
	}
	
	
	Tensor1f tf1(2);
	tf1(0)=1;
	tf1(1)=2;
	
	Log(2) << "Fuck insects1 " << tf1;

	const auto tfk = tf1;
	Log(2) << "Fuck insectsA";
	Tensor1f tf2 = tfk;
	//Tensor1f tf2 = tf1.clone();
//std::cout << "decltype(i) is " << type_name<decltype(tf1.clone())>() << '\n';

	Log(2) << "Fuck insects2";
	tf2.view()=1;
	Log(3) << tf1 << tf2;
	
	tf2 = Tensor1f(cv::Mat1f(3,2,1.f));
	Log(3) << tf1 << tf2;
	
	Log(1) << "AAAAAAAAAAAAAAAAAAAAAAAA1";
	
	Tensor1f tf = Tensor1f(2,3,4).view();

	Log(1) << "AAAAAAAAAAAAAAAAAAAAAAAA2";
	
	tf.view({ {} , {1,2}, {} } );
	Log(1) << "AAAAAAAAAAAAAAAAAAAAAAAA3";
	
	cout << Tensor1f(cv::Mat1f(1,3,0.f)).dimensions().size() << endl;
	Log(1) << "AAAAAAAAAAAAAAAAAAAAAAAA4";

	const int sz[] = {3, 2, 3, 2};
	cout << Tensor1f(cv::Mat1f(4,sz,0.f)).dimensions().size() << endl;
	Log(1) << "AAAAAAAAAAAAAAAAAAAAAAAA5";
	cout << endl;
	cout << Tensor1f(cv::Mat1f(4,sz,0.f))(1,1) << endl << endl<< endl<< endl;
	Log(1) << "AAAAAAAAAAAAAAAAAAAAAAAA6";
	
	cv::Mat1f f(4,sz,0.f);
	int i=0;
	for (auto &v : f) v = i++;
	
	
	
	Tensor1f T(f);
	//T(2,2) = cv::Mat1f::ones(3,6);
	
//	cout << T << endl;
	cout << T.view({{},{},{0,2},{0,1}}) << endl;
	
	Log(1) << "AAAAAAAAAAAAAAAAAAAAAAAA7";
	
	cout << T << endl;
	
	Log(1) << "AAAAAAAAAAAAAAAAAAAAAAAA7P";
	
	Log(2) << Tensor1f(cv::Mat1f(3,2,1.f));
	T(2,1).view() = Tensor1f(cv::Mat1f(3,2,1.f));
	Log(1) << "AAAAAAAAAAAAAAAAAAAAAAAA8";

	T = T.append(3,T);
	
	Log(1) << "AAAAAAAAAAAAAAAAAAAAAAAA8P";
	
	T(0,0,0,0) = 1000.;
	Log(1) << "AAAAAAAAAAAAAAAAAAAAAAAA9";

	cout << T << endl;
	Log(1) << "AAAAAAAAAAAAAAAAAAAAAAAA10";
	//(ofstream("test.th7")) << TorchMatrix(f);*/
}
