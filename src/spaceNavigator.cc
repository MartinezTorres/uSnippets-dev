////////////////////////////////////////////////////////////////////////
// minimal SpaceNavigator driver
//
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// If you need root permissions do:
//    echo -e 'SUBSYSTEM=="usb", ATTR{idVendor}=="046d", ATTR{idProduct}=="c626", MODE="0666"' | sudo tee /etc/udev/rules.d/51-SpaceNavigator.rules
//
// FLAGS: -g -std=c++11 `pkg-config opencv --cflags --libs`

#include <uSnippets/spaceNavigator.hpp>

using namespace std;
using uSnippets::SpaceNavigator;

int main() {
	
	{
		SpaceNavigator sn;
		sleep(1);
	}

	{
		SpaceNavigator sn;
		
		while (true) {
			
			cv::Mat1f translation, rotation;
			sn.get(translation, rotation);
			cout << translation << endl;
			sleep(1);
		}
	}
	
	return 0;
}

// Thanks for R.T.F.C.

