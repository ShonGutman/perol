#include "client.h"

int main()
{
	cout << "Welcome to perol!" << endl;
	
	// initializing io service
	ba::io_service ioService;
	try
	{
		// creating client 
		client p2pClient(ioService);

		// starting client
		p2pClient.start();
	}
	catch (std::exception& e)
	{
		std::cerr << "Error: " << e.what() << endl;
		return -1;
	}

	return 0;
}
