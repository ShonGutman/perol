#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <thread>
#include<mutex>
#include <string>
#include <iostream>
#include <unordered_map>
#include <chrono>

namespace ba = boost::asio;
namespace bs = boost::system;

//using clock = std::chrono::steady_clock;
using timePoint = std::chrono::time_point<std::chrono::steady_clock>;

using boost::asio::ip::udp;

using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::thread;
using std::unordered_map;

#define UDP_PORT 8200

struct client {
	udp::endpoint socketClient;
	timePoint lastTime;

	client() = default;

	client(udp::endpoint sock, timePoint time) : 
		socketClient(std::move(sock)), lastTime(time) {}
};

struct receivedMsg {
	string msgBuffer;
	udp::endpoint remoteEndpoint;
	bs::error_code error;
	size_t msgSize;
	timePoint receiveTime;

	receivedMsg(char buffer[1024], udp::endpoint sock, bs::error_code er, size_t size, timePoint time) :
	msgBuffer(buffer, size), remoteEndpoint(std::move(sock)), error(er), msgSize(size), receiveTime(time) {}
};

class server
{
public:
	server(ba::io_service& io_service);
	~server() = default;

	void run();

private:
	udp::socket _socketServer;
	unordered_map<std::string, client> _clientsMap;

	void startListening();
	void handleMsg(receivedMsg msgData);

	void sendFailed(udp::endpoint& remoteEndPoint);
	void sendMsg(const string& msg, const string& clientId);
	void sendMsg(const string& msg, udp::endpoint& remoteEndPoint);

	const string getIpPortString(const udp::endpoint& remoteEndPoint);

	void checkInactiveClients();
};