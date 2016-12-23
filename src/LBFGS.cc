////////////////////////////////////////////////////////////////////////
// minimal Levenberg-Marquardt code
//
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs`

#include <uSnippets/LBFGS.hpp>

using namespace std;

int main() {

	// More complex rosenbrock sample
	{
		cv::Mat1d X = (cv::Mat1d(2,1) << -1 , -2);
		bool b = LBFGS()( X, [](const cv::Mat1d &x)->double {
			
			cout << x(0) << " " << x(1) << " " << (1-x(0))*(1-x(0))+100*(x(1)-x(0)*x(0))*(x(1)-x(0)*x(0)) << endl;
			
			return (1-x(0))*(1-x(0))+100*(x(1)-x(0)*x(0))*(x(1)-x(0)*x(0));
		});
		cout << "Converged:" << (b?"yes":"no") << " with x=" << X << endl;
	}
	
	return 0;
}
