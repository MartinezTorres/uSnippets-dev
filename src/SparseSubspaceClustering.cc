////////////////////////////////////////////////////////////////////////
// minimal low Rank clustering code
//
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs` -DUSELAPACKSVD -llapack -fopenmp -lgomp



using namespace std;
#include <uSnippets/SparseSubspaceClustering.hpp>

int main() {
	
	int coverall = 0, total = 0;
	int P, F;
	while (cin >> P >> F) {
		
		cout << P << " " << F << endl;
		cv::Mat1i labels(P,1);
		cv::Mat1d D(2*F,P,0.);
		
		int nClasses = 0;
		for (int i=0; i<P; i++) {
			double dummy;
			for (int j=0; j<F; j++)
				cin >> D(2*j+0,i) >> D(2*j+1,i) >> dummy;
			cin >> labels(i);
			nClasses = max(nClasses, labels(i));
			labels(i)--;
		}
	
		SparseSubspaceClustering ssc(D, nClasses+1, 3, labels);

		for (int i=0; i<D.cols; i++) cout << ssc.labels(i) << flush; cout << endl;
		for (int i=0; i<D.cols; i++) cout << labels(i) << flush; cout << endl;
		
		int correct = 0;
		for (int i=0; i<D.cols; i++)
			correct += labels(i)==ssc.labels(i);
		cout << 100.*correct/D.cols << "%" << endl;
		coverall += correct;
		total += D.cols;
		//sleep(1);
	}
	cout << 100.*coverall/total << "%" << endl;
}
