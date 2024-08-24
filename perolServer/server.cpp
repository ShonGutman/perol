#include "server.h"

// server commands
#define EXIT "exit" // ends server run

server::server(boost::asio::io_service& io_service) :
	_socketServer(io_service, udp::endpoint(udp::v4(), UDP_PORT)) // initialize server socket
{
}

void server::run()
{
	// creating listening thread
	thread t_listen(&server::startListening, this);
	t_listen.detach();


	std::string input;
	while (input != EXIT)
	{
		// get commands from console
		std::getline(std::cin, input);

		if (input == EXIT)
			cout << "Goodbye!" << endl;
		else
			cout << "Please enter a suported command." << endl;
	}
}

void server::startListening()
{
	while(true)
	{
		// initializing client data
		char recvBuffer[1024] = { 0 };
		udp::endpoint remoteEndpoint;
		bs::error_code error;

		cout << endl << "Listening..." << endl;

		// waiting for client
		_socketServer.receive_from(ba::buffer(recvBuffer),
			remoteEndpoint, 0, error);

		// error while listening
		if (error && error != ba::error::message_size)
		{
			cerr << "Error while listening: " << error.message() << endl;
			return;
		}

		// create new Thread to handle the msg
		thread t_newClient(&server::handleMsg, this, std::move(remoteEndpoint));
		t_newClient.detach();
	}
}

void server::handleMsg(udp::endpoint remoteEndpoint)
{
	string clientId = getIpPortString(remoteEndpoint);

	try
	{
		if (_clientsMap.find(clientId) == _clientsMap.end())
		{
			// New client
			_clientsMap.emplace(clientId, remoteEndpoint);
			handleNewClient(clientId);
		}
		else
			// Existing client
			handleExistingClient(clientId);
	}
	catch (std::exception& e)
	{
		cout << "Exception: " << e.what() << endl;
	}
	catch (...)
	{
		cout << "Something went terribly wrong, and I have no idea what." << endl;
	}
}

void server::handleNewClient(const string clientId)
{
	cout << "New client accepted " << clientId << endl;

	sendMsg("Welcome!\n", clientId);
}

void server::handleExistingClient(const string clientId)
{
	cout << "Existing client reached " << clientId << endl;

	sendMsg("Good to see you again!\n", clientId);
}

void server::sendMsg(const string& msg, const string& clientId)
{
	if (_clientsMap.find(clientId) != _clientsMap.end())
	{
		bs::error_code ignored_error;
		_socketServer.send_to(ba::buffer(msg),
			_clientsMap[clientId], 0, ignored_error);
	}
	else
		throw clientId;
}

const string server::getIpPortString(const udp::endpoint& remoteEndpoint)
{
	return remoteEndpoint.address().to_string() + ":" + std::to_string(remoteEndpoint.port());
}
