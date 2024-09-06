#pragma once

#include <string>
#include <boost/asio.hpp>
#include <chrono>

using std::string;

//using clock = std::chrono::steady_clock;
using timePoint = std::chrono::time_point<std::chrono::steady_clock>;
using boost::asio::ip::udp;

//general info of a request 
struct RequestInfo
{
	string msg;
	udp::endpoint remoteEndpoint;
	timePoint receiveTime;

	RequestInfo(char* buffer, udp::endpoint& sock, timePoint& time) :
		msg(buffer), remoteEndpoint(std::move(sock)), receiveTime(std::move(time)) {}
};

//general result of request
struct RequestResult
{
	string response;

	RequestResult(const string& response) :
		response(response) {}
};
