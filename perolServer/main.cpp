#include "server.h"
#include <iostream>

int main()
{
    cout << "Initializing io service" << endl;
    ba::io_service io_service;

    cout << "Initializing server" << endl;
    server p2pServer(io_service);

    cout << "Starting running server" << endl;
    cout << "--------------------" << endl;
    p2pServer.run();

    cout << endl << "Successfully run" << endl;
    return 0;
}