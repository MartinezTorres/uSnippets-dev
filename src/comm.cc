////////////////////////////////////////////////////////////////////////
// Manuel Martinez (manuel.martinez@kit.edu)
//
// license: LGPLv3
//
// To test, in one terminal run: nc.traditional -l -p 8888 -e bin/comm
// And in other: nc.traditional -e bin/comm localhost 8888
//
// FLAGS: -g -std=c++11 -pthread -lboost_system

#include <uSnippets/comm.hpp>

#include <iostream>
using namespace std;
using namespace uSnippets::comm;
using uSnippets::object;
using uSnippets::Log;

using uS = std::chrono::microseconds;

static const int rate = 5;

struct Server {
	bool alive = true;
	std::string id;
	thread t = thread([this](){

		// I'll send packed messages, but receive unpacked orders
		Log(1) << "Server " << id << " Thread";
		Net::Server netserver; 
		
		Message m;
		while (alive) {
			if (netserver.pop(m)) {
				Log(1) << "Server received: " << m.ID();
				netserver << m;
			}
		}
	});
	Server(std::string id) : id(id) {Log(1) << "Server " << id << " STARTING";}
	~Server() { alive=false; t.join(); Log(1) << "Server " << id << " LEAVING";}
};

struct Client {
	bool alive = true;
	std::string id;
	thread t = thread([this](){

		// I'll send packed unpacked messages, but receive packed messages
		Log(1) << "Client " << id << " THREAD";
		Net::Client netclient("localhost");
		
		int nMsg = 0;
		while (alive and nMsg<1000000) {
			Message m;
			while (netclient.pop(m,uS(1000000/rate))) 
				Log(1) << "I'm " << id << ": Howdy! Received " << m.ID();
			
			if (netclient.push(Message(object(id) <<" " << nMsg++) << string(1000000,'a')))
				Log(1) << "I'm " << id << ": Howdy! Sent " << nMsg;
		}
		Log(1) << "Client " << id << " THREAD Finished";
	});
	Client(std::string id) : id(id) {Log(1) << "Client " << id << " STARTING";}
	~Client() { alive=false; t.join(); Log(1) << "Client " << id << " LEAVING";}
};

int main() {

	Log::reportLevel(-2);

	if (1) {
		Server st("S");

		std::this_thread::sleep_for( uS(500000) );

		Client a("A");
		std::this_thread::sleep_for( uS(500000) );
		Client b("B");

		while (true) std::this_thread::sleep_for( uS(100000000) );
	} else {
		
		Log(1) << "Starting a single Client";
		{ 
			Client test1("Test1");
			std::this_thread::sleep_for( uS(3000000) );
		}
		Log(1) << "Starting a Client before a server";
		{ 
			Client test2("Test2");
			std::this_thread::sleep_for( uS(1000000) );
			{
				Server s1("S1");
				std::this_thread::sleep_for( uS(1000000) );
			}
			std::this_thread::sleep_for( uS(1000000) );
			{
				Server s2("S2");
				std::this_thread::sleep_for( uS(2000000) );
			}
			std::this_thread::sleep_for( uS(1000000) );
		}
		
		
	}
	return 0;
}
