#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <thread>
#include <chrono>

using boost::asio::ip::udp;

namespace ba = boost::asio;

using std::cout;
using std::endl;
using std::string;

class client
{
public:
	client(ba::io_service& ioService);
	~client();
	
	void start();

	void keepAlive();

	string receiveMsgServer();
	void sendMsgServer(const string& msg);

private:
	udp::socket _clientSock;
	udp::endpoint serverSock;
};