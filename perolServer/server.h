#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
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

class server
{
public:
	server() = default;
	~server() = default;

	void run();

private:

	void startListening();
};