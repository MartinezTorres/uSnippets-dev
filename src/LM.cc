////////////////////////////////////////////////////////////////////////
// minimal Levenberg-Marquardt code
//
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs`

#include <uSnippets/LM.hpp>

using namespace std;

int main() {

	// Easy sample
	{
		cv::Mat1d X = cv::Mat1d::zeros(3,1);
		bool b = LM()( X, [&](const cv::Mat1d &x)->cv::Mat1d {

			function<double(double)> f1 = [&](double xx){ return sin(xx); };
			function<double(double)> f2 = [&](double xx){ return xx+x(0)*pow(xx,3)+x(1)*pow(xx,5)+x(2)*pow(xx,7); };
			
			cv::Mat1d Y(4,1);
			Y(0) = f1(0.1) - f2(0.1);
			Y(1) = f1(0.2) - f2(0.2);
			Y(2) = f1(0.3) - f2(0.3);
			Y(3) = f1(0.4) - f2(0.4);
			
			return Y;
		});
		
		cout << "Converged:" << (b?"yes":"no") << " with x=" << X << endl;
	}

	// More complex rosenbrock sample
	{
		cv::Mat1d X = (cv::Mat1d(2,1) << -3 , -4);
		bool b = LM()( X, [&](const cv::Mat1d &x)->cv::Mat1d {
			
			cv::Mat1d Y(1,1);
			Y(0) = (1-x(0))*(1-x(0))+100*(x(1)-x(0)*x(0))*(x(1)-x(0)*x(0));
			return Y;
		});
		cout << "Converged:" << (b?"yes":"no") << " with x=" << X << endl;
	}
	
	return 0;
}
