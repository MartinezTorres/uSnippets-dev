////////////////////////////////////////////////////////////////////////
// minimal low Rank clustering code
//
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs` -DUSELAPACKSVD -llapack



using namespace std;
#include <uSnippets/LowRankClustering.hpp>

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
	
		LowRankClustering lrc(D, nClasses, 2, labels, 422);

		for (int i=0; i<D.cols; i++) cout << lrc.labels(i) << flush; cout << endl;
		
		for (int i=0; i<D.cols; i++) cout << abs(labels(i)-lrc(D.col(i))) << flush; cout << endl;
		
		int correct = 0;
		for (int i=0; i<D.cols; i++)
			correct += labels(i)==lrc.labels(i);
//			correct += labels(i)==lrc(D.col(i));
		cout << 100.*correct/D.cols << "%" << endl;
		coverall += correct;
		total += D.cols;
	}
	cout << 100.*coverall/total << "%" << endl;
}
