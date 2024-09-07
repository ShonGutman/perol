#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <thread>
#include <string>
#include <iostream>
#include <chrono>

#include "Client.h"
#include "StatusHandler.h"

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

#define UDP_PORT 8200
#define MSG_LENGTH 1024

class server
{
public:
	server(ba::io_service& io_service);
	~server() = default;
	
	/*
	* function runs the server and initializes all threads to run
	*/
	void run();

private:
	udp::socket _socketServer;

	/*
	* function listens to input and if it recieces valid client it creates thread to handle his request
	*/
	void startListening();

	/*
	* function handels client request
	* @param info - info of client's request
	*/
	void handleMsg(RequestInfo info);

	/*
	* function sends msg to a given udp endpoint
	* @param msg - msg to be sent
	* @param remoteEndPoint - endpoint of client to send data to
	*/
	void sendMsg(const string& msg, udp::endpoint& remoteEndPoint);

	/*
	* function converts udp endpoint data into string with ip and port
	* @param remoteEndpoint - endpoint of client
	* @return string with ip and port
	*/
	string getIpPortString(const udp::endpoint& remoteEndpoint);
};