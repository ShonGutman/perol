#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <thread>
#include <string>
#include <iostream>
#include <unordered_map>

namespace ba = boost::asio;
namespace bs = boost::system;

using boost::asio::ip::udp;

using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::thread;
using std::unordered_map;

#define UDP_PORT 8200

class server
{
public:
	server(ba::io_service& io_service);
	~server() = default;

	void run();

private:
	udp::socket _socketServer;
	unordered_map<std::string, udp::endpoint> _clientsMap;

	void startListening();
	void handleMsg(udp::endpoint remoteEndpoint);

	void handleNewClient(const string clientId);
	void handleExistingClient(const string clientId);

	void sendMsg(const string& msg, const string& clientId);

	const string getIpPortString(const udp::endpoint& remoteEndpoint);
};