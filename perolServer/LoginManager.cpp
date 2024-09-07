#include "LoginManager.h"

#include <mutex>
#include <iostream>
#include <chrono>
#include <thread>

using std::cout;
using std::endl;
using std::cerr;

static std::mutex _clientsMutex;
extern std::mutex _coutMutex;
extern bool exitFlag;

LoginManager& LoginManager::get()
{
	static LoginManager instance;
	return instance;
}

bool LoginManager::exists(const string& clientId) const
{
	//lock the mutex - to protect _clientsMap (shared var)
	std::lock_guard<std::mutex> locker(_clientsMutex);
	return _clientsMap.find(clientId) != _clientsMap.end();
}

RequestId LoginManager::getCurrentHandler(const string& clientId) const
{
	//lock the mutex - to protect _clientsMap (shared var)
	std::lock_guard<std::mutex> locker(_clientsMutex);
	return _clientsMap.at(clientId).handler;
}

void LoginManager::insert(const string& clientId, const Client& client)
{
	//lock the mutex - to protect _clientsMap (shared var)
	std::lock_guard<std::mutex> locker(_clientsMutex);
	_clientsMap.emplace(clientId, client);
}

void LoginManager::erase(const string& clientId)
{
	//lock the mutex - to protect _clientsMap (shared var)
	std::lock_guard<std::mutex> locker(_clientsMutex);
	_clientsMap.erase(clientId);
}
