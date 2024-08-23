#include "server.h"
#include <iostream>

int main()
{
    cout << "Initializing server" << endl;
    server p2pServer;

    cout << "Starting running server" << endl;
    cout << "--------------------" << endl;
    p2pServer.run();

    cout << endl << "Successfully run" << endl;
    return 0;
}