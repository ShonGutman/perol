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

void LoginManager::updateLastTime(const string& clientId, const timePoint& newTime)
{
	//lock the mutex - to protect _clientsMap (shared var)
	std::lock_guard<std::mutex> locker(_clientsMutex);
	_clientsMap.at(clientId).lastTime = newTime;
}

void LoginManager::removeInactiveClients()
{
	while (!exitFlag)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));

		timePoint now = std::chrono::steady_clock::now();

		//lock the mutex - to protect _clientsMap (shared var)
		std::lock_guard<std::mutex> locker(_clientsMutex);
		for (auto client = _clientsMap.begin(); client != _clientsMap.end();)
		{
			if (std::chrono::duration_cast<std::chrono::seconds>(now - client->second.lastTime).count() > 7)
			{
				{
					//lock the mutex - to protect _coutMutex (shared var)
					std::lock_guard<std::mutex> lock(_coutMutex);
					cout << "Timed out client " << client->first << endl;
				}

				client = _clientsMap.erase(client);
			}

			else
			{
				client++;
			}
		}
	}
}
