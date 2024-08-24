#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <thread>
#include <string>
#include <iostream>

namespace ba = boost::asio;
namespace bs = boost::system;

using boost::asio::ip::udp;

using std::cout;
using std::endl;
using std::string;
using std::thread;

#define UDP_PORT 8200

class server
{
public:
	server(ba::io_service& io_service);
	~server() = default;

	void run();

private:
	udp::socket _socketServer;

	void startListening();
	void handleNewClient(udp::endpoint remoteEndpoint);
	void sendMsg(const string& msg, udp::endpoint& remoteEndpoint);
	string receiveMsg(udp::endpoint& remoteEndpoint);
};