#pragma once

#include "LoginRequestHandler.h"
#include "LoginManager.h"
#include "MenuRequestHandler.h"


class StatusHandler
{
public:

	// CTOR //
	StatusHandler() = default;

	/*
	* function returns the current handler of a given client
	* @param clientId - id of client that we want to recieve his requestHandler
	* @return - pointer to a handler - memory must be deleted!
	*/
	IRequestHandler* handleStatus(const string& clientId);

private:

};
