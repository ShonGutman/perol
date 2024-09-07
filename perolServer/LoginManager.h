#pragma once

#include <map>
#include <string>

#include "Client.h"

using std::string;

class LoginManager
{
public:

	/*
	* Function to get the singleton instance
	*
	* @param database the database needed, not a must
	* @return the instance of LoginManager
	*/
	static LoginManager& get();

	// Delete copy constructor and assignment operator to prevent copies
	LoginManager(const LoginManager&) = delete;
	LoginManager& operator=(const LoginManager&) = delete;

	/*
	* function checks if user exists in map
	* @param clientId -  unique id of client
	* @return true if exists
	*/
	bool exists(const string& clientId) const;

	/*
	* function returns the current Request Handler id of specific client
	* @param clientId - id of client to recieve handler
	* @return id value of current client handler
	*/
	RequestId getCurrentHandler(const string& clientId) const;

	/*
	* function inserts new client to map
	* @param clientId - id of new client to be added
	* @param client - Client struct with all data regarding that client
	*/
	void insert(const string& clientId, const Client& client);

	/*
	* function removes given client from map
	* @param clientId - id of client to be removed
	*/
	void erase(const string& clientId);

private:

	// Private constructor to prevent external instantiation
	// CTOR //
	LoginManager();

	std::map<string, Client> _clientsMap;

};