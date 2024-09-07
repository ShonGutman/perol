#include "MenuRequestHandler.h"

#define KA "KEEP ALIVE"
#define LEAVE "LEAVE"

// server messages
#define FAILED "FAILED"
#define RECEIVED "RECEIVED"
#define BYE "BYE"

extern std::mutex _coutMutex;

RequestResult MenuRequestHandler::handleRequest(RequestInfo& info, const string& clientId)
{
    if (info.msg == KA)
    {
        LoginManager& loginManager = LoginManager::get();

        loginManager.updateLastTime(clientId, info.receiveTime);

        {
            //lock the mutex - to protect _coutMutex (shared var)
            std::lock_guard<std::mutex> lock(_coutMutex);
            cout << "Existing client reached " << clientId << endl;
        }

        return RequestResult(RECEIVED);
    }

    if (info.msg == LEAVE)
    {
        LoginManager& loginManager = LoginManager::get();

        loginManager.erase(clientId);

        {
            //lock the mutex - to protect _coutMutex (shared var)
            std::lock_guard<std::mutex> lock(_coutMutex);
            cout << "Removed client " << clientId << endl;
        }

        return RequestResult(BYE);
    }

    return RequestResult(FAILED);
}
