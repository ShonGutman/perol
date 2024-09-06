#pragma once

#include <boost/asio.hpp>
#include <chrono>

//using clock = std::chrono::steady_clock;
using timePoint = std::chrono::time_point<std::chrono::steady_clock>;
using boost::asio::ip::udp;

typedef enum : unsigned int
{
	LOGIN_REQUEST_HANDLER = 1,
	MENU_REQUEST_HANDLER = 2
}RequestId;

struct Client 
{
	udp::endpoint socketClient;
	timePoint lastTime;
	RequestId handler;

	Client() = default;

	Client(udp::endpoint& sock, timePoint& time) :
		socketClient(std::move(sock)), lastTime(std::move(time)) 
	{
		handler = MENU_REQUEST_HANDLER;
	}
};
