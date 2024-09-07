#include "server.h"

#define EXIT "exit" // ends server run

bool exitFlag = false;
std::mutex _coutMutex;

server::server(boost::asio::io_service& io_service) :
	_socketServer(io_service, udp::endpoint(udp::v4(), UDP_PORT)) // initialize server socket
{
}

void server::run()
{
	// creating listening thread
	thread t_listen(&server::startListening, this);

	// create thread to remove inactive clients
	thread t_inactiveClients(&LoginManager::removeInactiveClients, std::ref(LoginManager::get()));

	string input;
	while (input != EXIT)
	{
		// get commands from console
		std::getline(std::cin, input);

		if (input == EXIT)
		{
			cout << "Goodbye!" << endl;
			exitFlag = true;
			_socketServer.close();
			t_listen.join();
			t_inactiveClients.join();
		}

		else
		{
			cout << "Please enter a suported command." << endl;
		}
	}
}

void server::startListening()
{
	{
		//lock the mutex - to protect _coutMutex (shared var)
		std::lock_guard<std::mutex> lock(_coutMutex);
		cout << endl << "Listening..." << endl;
	}

	while(!exitFlag)
	{
		// initializing msg data
		char recvBuffer[MSG_LENGTH] = { '\0' };
		udp::endpoint remoteEndpoint;
		bs::error_code error;

		// waiting for client
		_socketServer.receive_from(ba::buffer(recvBuffer),
			remoteEndpoint, 0, error);

		// error while listening
		if (error && error != ba::error::message_size)
		{
			//lock the mutex - to protect _coutMutex (shared var)
			std::lock_guard<std::mutex> lock(_coutMutex);
			cerr << "Error while listening: " << error.message() << endl;
			return;
		}

		// initializing msg data struct
		timePoint now = std::chrono::steady_clock::now();
		RequestInfo info(recvBuffer, remoteEndpoint, now);

		// create new Thread to handle the msg
		thread t_newClient(&server::handleMsg, this, std::move(info));
		t_newClient.detach();

	}
}

void server::handleMsg(RequestInfo info)
{
	try
	{

		string clientId = getIpPortString(info.remoteEndpoint);
		StatusHandler status;

		//get the handler of the client
		IRequestHandler* request = status.handleStatus(clientId);

		RequestResult result = request->handleRequest(info, clientId);

		//delete allocated memory
		delete request;
		request = nullptr;

		sendMsg(result.response, info.remoteEndpoint);
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

void server::sendMsg(const string& msg, udp::endpoint& remoteEndPoint)
{
	bs::error_code error;
	_socketServer.send_to(ba::buffer(msg),
		remoteEndPoint, 0, error);

	// check error
}

string server::getIpPortString(const udp::endpoint& remoteEndpoint)
{
	return remoteEndpoint.address().to_string() + ":" + std::to_string(remoteEndpoint.port());
}
