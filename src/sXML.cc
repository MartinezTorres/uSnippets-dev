////////////////////////////////////////////////////////////////////////
// minimal sXML sample
//
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11

#include <uSnippets/sXML.hpp>

#include <iostream> 
using namespace std;
using namespace uSnippets;

int main() {
	
	sXML empty;
	cout << "Empty: " << empty << endl;

	sXML chief("chief");
	chief["rank"] = "general";
	chief["age"] = 57;

	chief("First")["rank"] = "captain";
	chief("First")["age"] = 27;

	chief("Second")["rank"] = "cook";
	chief("Second")["age"] = 2;

	sXML wife("wife");
	chief("Wife") = wife;

	cout << "Chief: " << endl << chief << endl;

	object str = chief;

	cout << "Serialized Chief: " << endl << str << endl;

	sXML chief2 = str.get<sXML>();

	cout << "Read Chief: " << endl << chief2 << endl;
	
}
