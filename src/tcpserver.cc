////////////////////////////////////////////////////////////////////////
// micro HTTP server
//
// Manuel Martinez (salutte@gmail.com)
//
// license: LGPLv3
//
// FLAGS: -g -std=c++11 -lboost_system -pthread

#include <uSnippets/tcpserver.hpp>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	
	int port = 80;
	if (argc>1) port = atoi(argv[1]);
	
	uSnippets::HTTPServer http(port);
	
	http("", [](boost::asio::ip::tcp::iostream &net, string get, string url, string host) {
		
		cerr << "Received: " << get << " " << url << " " << host << endl;
		net << "HTTP/1.1 200 OK\r\n";
		net << "Content-Type: text/html\r\n\r\n";
		net << "<html><body>Hai! " << url << "</body></html>\r\n";
		
		std::this_thread::sleep_for( std::chrono::seconds(1) );
	});
	
	int good = true;
	http("quit", [&](boost::asio::ip::tcp::iostream &net, string get, string url, string host) {
		
		cerr << "Received: " << get << " " << url << " " << host << endl;
		net << "HTTP/1.1 200 OK\r\n";
		net << "Content-Type: text/html\r\n\r\n";
		net << "<html><body> bye! </body></html>\r\n";
		
		good = false;
	});
	
	while (good) std::this_thread::sleep_for( std::chrono::milliseconds(1) );
	cerr << "I'm out!" << endl;
}
