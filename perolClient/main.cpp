#include "client.h"

int main()
{
	cout << "Welcome to perol!" << endl;
	
	ba::io_service ioService;
	client p2pClient(ioService);

	p2pClient.start();

	return 0;
}
