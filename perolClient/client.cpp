#include "client.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT "8200"

// client messages 
#define LOGIN "LOGIN"
#define KA "KEEP ALIVE"
#define BYE "BYE"

// server messages
#define SUCCEED "SUCCEED"
#define FAILED "FAILED"
#define RECEIVED "RECEIVED"

#define EXIT "EXIT"

static bool exitFlag = false;

client::client(ba::io_service& ioService) : 
	_clientSock(ioService, udp::endpoint(udp::v4(), 0))
{
	udp::resolver resolver(ioService);
	udp::resolver::query query(udp::v4(), SERVER_IP, SERVER_PORT);
	udp::resolver::iterator iter = resolver.resolve(query); // add try catch in case of error
	serverSock = *iter;
}

client::~client()
{
	// send BYE to server
	sendMsgServer(BYE);
	// close client socket
	_clientSock.close();
}

void client::start()
{
	sendMsgServer(LOGIN);

	receiveMsgServer(); // add check for SUCCEED

	std::thread t_keepAlive = std::thread(&client::keepAlive, this);

	std::string input;
	while (input != EXIT)
	{
		// get commands from console
		std::getline(std::cin, input);

		if (input == EXIT)
		{
			cout << "Goodbye!" << endl;
			exitFlag = true;
		}

		else
		{
			cout << "Please enter a suported command." << endl;
		}
	}

	t_keepAlive.join();
}

void client::keepAlive()
{
	while (!exitFlag)
	{
		sendMsgServer(KA);

		string serverMsg = receiveMsgServer();

		if (serverMsg == RECEIVED)
			std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}

string client::receiveMsgServer()
{
	char recBuffer[1024] = { '\0' };
	_clientSock.receive_from(ba::buffer(recBuffer), serverSock);
	return recBuffer;
}

void client::sendMsgServer(const string& msg)
{
	_clientSock.send_to(ba::buffer(msg, msg.size()), serverSock);
}
