////////////////////////////////////////////////////////////////////////
// simple GUI code
//
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// GCC-FLAGS: -g -Ofast -std=c++11 `pkg-config opencv --cflags` `pkg-config opencv --libs` -lSDL -lGLU -lGL -Wno-unused-parameter -Wno-unused-variable

#include <cstdint>
#include <chrono>
#include <iostream>
using namespace std;


	using microseconds = std::chrono::microseconds;
	using time_point = std::chrono::system_clock::time_point;
	constexpr auto &now = std::chrono::system_clock::now;
	
	
	//  Note that the behavior is slightly different than C++14, as all types are converted to floating point representation to ease conversion problems
	constexpr microseconds operator "" _h  (long double               t) { return microseconds(uint64_t(t*1e6*3600)); }
	constexpr microseconds operator "" _h  (unsigned long long int    t) { return microseconds(uint64_t(t*1e6*3600)); }
	constexpr microseconds operator "" _min(long double               t) { return microseconds(uint64_t(t*1e6*60)); }
	constexpr microseconds operator "" _min(unsigned long long int    t) { return microseconds(uint64_t(t*1e6*60)); }
	constexpr microseconds operator "" _s  (long double               t) { return microseconds(uint64_t(t*1e6)); }
	constexpr microseconds operator "" _s  (unsigned long long int    t) { return microseconds(uint64_t(t*1e6)); }
	constexpr microseconds operator "" _ms (long double               t) { return microseconds(uint64_t(t*1e3)); }
	constexpr microseconds operator "" _ms (unsigned long long int    t) { return microseconds(uint64_t(t*1e3)); }
	constexpr microseconds operator "" _us (long double               t) { return microseconds(uint64_t(t)); }
	constexpr microseconds operator "" _us (unsigned long long int    t) { return microseconds(uint64_t(t)); }

int main() {
	



	microseconds ld;
	
	ld = 1_ms-10_s;
	
	cout << ld.count() << endl;
	
}
