#include "server.h"

// client messages 
#define LOGIN "LOGIN"
#define KA "KEEP ALIVE"

// server messages
#define SUCCEED "SUCCEED"
#define FAILED "FAILED"
#define RECEIVED "RECEIVED"

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

	// create thread to remove inactive clients
	thread t_inactiveClients(&server::checkInactiveClients, this);
	t_inactiveClients.detach();

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
		// initializing msg data
		char recvBuffer[1024] = { '\0' };
		udp::endpoint remoteEndpoint;
		bs::error_code error;

		cout << endl << "Listening..." << endl;

		// waiting for client
		size_t size = _socketServer.receive_from(ba::buffer(recvBuffer),
			remoteEndpoint, 0, error);

		// error while listening
		if (error && error != ba::error::message_size)
		{
			cerr << "Error while listening: " << error.message() << endl;
			return;
		}

		// initializing msg data struct
		receivedMsg msgData(recvBuffer, std::move(remoteEndpoint), error, size, std::chrono::steady_clock::now());

		// create new Thread to handle the msg
		thread t_newClient(&server::handleMsg, this, msgData);
		t_newClient.detach();
	}
}

void server::handleMsg(receivedMsg msgData)
{
	try
	{
		if (msgData.msgBuffer == LOGIN)
		{
			string clientId = getIpPortString(msgData.remoteEndpoint);
			if (_clientsMap.find(clientId) == _clientsMap.end())
			{
				// add new client to client list
				_clientsMap.emplace(clientId, client(std::move(msgData.remoteEndpoint), msgData.receiveTime));

				cout << "New client accepted " << clientId << endl;

				sendMsg(SUCCEED, clientId);
			}
			else
				sendFailed(msgData.remoteEndpoint);
		}
		else if (msgData.msgBuffer == KA)
		{
			string clientId = getIpPortString(msgData.remoteEndpoint);
			auto clientIt = _clientsMap.find(clientId);
			if( clientIt != _clientsMap.end())
			{
				_clientsMap.at(clientId).lastTime = msgData.receiveTime;

				cout << "Existing client reached " << clientId << endl;

				sendMsg(RECEIVED, clientId);
			}
		}
		else
		{
			sendFailed(msgData.remoteEndpoint);
		}
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

void server::sendFailed(udp::endpoint& remoteEndPoint)
{
	sendMsg(FAILED, remoteEndPoint);
}

void server::sendMsg(const string& msg, const string& clientId)
{
	if (_clientsMap.find(clientId) != _clientsMap.end())
	{
		sendMsg(msg, _clientsMap[clientId].socketClient);
	}
	else
		throw clientId;
}

void server::sendMsg(const string& msg, udp::endpoint& remoteEndPoint)
{
	bs::error_code error;
	_socketServer.send_to(ba::buffer(msg),
		remoteEndPoint, 0, error);

	// check error
}

const string server::getIpPortString(const udp::endpoint& remoteEndpoint)
{
	return remoteEndpoint.address().to_string() + ":" + std::to_string(remoteEndpoint.port());
}

void server::checkInactiveClients()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));

		timePoint now = std::chrono::steady_clock::now();
		for (auto client = _clientsMap.begin(); client != _clientsMap.end(); ++client)
		{
			if (std::chrono::duration_cast<std::chrono::seconds>(now - client->second.lastTime).count() > 7)
			{
				cout << "Timed out client " << client->first << endl;
				_clientsMap.erase(client);
			}
		}
	}
}
