#include "server.h"

// server commands
#define EXIT "exit" // ends server run

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
	
}
