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
	for (;;)
	{
		// initializing client data
		boost::array<char, 1> recvBuffer;
		udp::endpoint remoteEndpoint;
		bs::error_code error;

		cout << endl << "Listening..." << endl;

		// waiting for client
		_socketServer.receive_from(ba::buffer(recvBuffer),
			remoteEndpoint, 0, error);

		// error while listening
		if (error && error != ba::error::message_size)
			throw bs::system_error(error);

		// creating client thread
		thread t_newClient(&server::handleNewClient, this, std::move(remoteEndpoint));
		t_newClient.detach();
	}
}

void server::handleNewClient(udp::endpoint remoteEndpoint)
{
	cout << "new client!" << endl;
}
