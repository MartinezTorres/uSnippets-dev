////////////////////////////////////////////////////////////////////////
// minimal Levenberg-Marquardt code
//
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs` -fopenmp -lgomp


#include <uSnippets/LassoShooting.hpp>
#include <uSnippets/OWLQN.hpp>
#include <uSnippets/LBFGS.hpp>
#include <fstream>

using namespace std;

cv::Mat1d LassoShotgun(cv::Mat1d X_, cv::Mat1d Y, double lambda, cv::Mat1b mask = cv::Mat1b());

static inline cv::Mat1d readOctave(string file) {
	
	fstream fin(file);
	string s;
	int cols, rows;
	while (fin>>s and s!="rows:");
	fin >> rows;
	while (fin>>s and s!="columns:");
	fin >> cols;
	cv::Mat1d R(rows, cols);
	for (int i=0; i<rows; i++)
		for (int j=0; j<cols; j++)
			fin >> R(i,j);
	
	return R;
}

int main() {
	
	double l = .001;


	cv::Mat1d D(16,1024);
	for (int i=0; i<D.rows; i++)
		for (int j=0; j<D.cols; j++)
			D(i,j) = .5-(rand()%10000)/10000.;
			
	cv::Mat1d Y(D.rows,1);
	for (int i=0; i<Y.rows; i++)
		Y(i) = .5-(rand()%10000)/10000.;

//	cv::Mat1d D = readOctave("X.txt");
//	cv::Mat1d Y = readOctave("Y.txt");
//	cv::Mat1d B = readOctave("B.txt");
//	cout << endl << cv::norm(D*B-Y) << " " << cv::norm(B,cv::NORM_L1) << endl;
	
	
	
	cout << "Start" << endl;
	for (int i=0; i<100; i++) {
		cv::Mat1d B1 = LassoShooting( D, Y, l);
		if (not i) cout << endl << cv::norm(Y) << " " << cv::norm(D*B1-Y) << " " << cv::norm(B1,cv::NORM_L1) << endl;
	}
	cout << "Shooting done" << endl;
	
	
	for (int i=0; i<100; i++) {
		cv::Mat1d B2(D.cols,1,0.);
		//cv::solve( D , Y, B2, cv::DECOMP_SVD);
		(OWLQN(l))( 
			B2, 
			[&](const cv::Mat1d &B2)->double   { cv::Mat1d N = D*B2-Y; return N.dot(N); },
			[&](const cv::Mat1d &B2)->cv::Mat1d { return 2*D.t()*(D*B2-Y); }
		);
		if (not i) cout << cv::norm(D*B2-Y) << " " << cv::norm(B2,cv::NORM_L1) << endl;
	}
	cout << "OWLQN Done" << endl;
	
	
	
	
	if (0)
	for (int i=0; i<100; i++) {
		cv::Mat1d B3(D.cols,1,0.);
		//cv::solve( D , Y, B3, cv::DECOMP_SVD);
		LBFGS()( B3, [&](const cv::Mat1d &B3)->double {
			return pow(cv::norm(D*B3-Y),2) + l*cv::norm(B3,cv::NORM_L1);
		});
		if (not i) cout << cv::norm(D*B3-Y) << " " << cv::norm(B3,cv::NORM_L1) << endl;
	}
	cout << "LBFGS Done" << endl;


	/*for (int i=0; i<100; i++) {
		cv::Mat1d B4 = LassoShotgun( D, Y, l );
		if (not i) cout << endl << cv::norm(D*B4-Y) << " " << cv::norm(B4,cv::NORM_L1) << endl;
	}
	cout << "LassoShotgun Done" << endl;*/
		
	return 0;
}
