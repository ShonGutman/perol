#pragma once

#include <iostream>

#include "Requests.h"
#include "LoginManager.h"

using std::cout;
using std::endl;
using std::cerr;


class IRequestHandler
{
public:

	virtual RequestResult handleRequest(RequestInfo& info, const string& clientId) = 0;

};