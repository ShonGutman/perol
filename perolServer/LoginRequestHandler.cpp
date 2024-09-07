#include "LoginRequestHandler.h"

#define LOGIN "LOGIN"

// server messages
#define SUCCEED "SUCCEED"
#define FAILED "FAILED"

extern std::mutex _coutMutex;

RequestResult LoginRequestHandler::handleRequest(RequestInfo& info, const string& clientId)
{
    if (info.msg == LOGIN)
    {
        LoginManager& loginManager = LoginManager::get();

        loginManager.insert(clientId, Client(info.remoteEndpoint, info.receiveTime));

        {
            //lock the mutex - to protect _coutMutex (shared var)
            std::lock_guard<std::mutex> lock(_coutMutex);
            cout << "New client accepted " << clientId << endl;
        }

        return RequestResult(SUCCEED);
    }

    return RequestResult(FAILED);
}
