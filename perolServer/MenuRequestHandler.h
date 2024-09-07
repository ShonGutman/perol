#pragma once

#include "IRequestHandler.h"

class MenuRequestHandler : public IRequestHandler
{
public:

	/*
	* function handels menu request and returns proper result according to input
	* @param info - an info struct containing data on client
	* @param clientId - client's unique id
	* @return RequestResult struct containg data that should be returned to user
	*/
	RequestResult handleRequest(RequestInfo& info, const string& clientId) override;

};
