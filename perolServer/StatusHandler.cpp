#include "StatusHandler.h"

IRequestHandler* StatusHandler::handleStatus(const string& clientId)
{
	const LoginManager& loginManager = LoginManager::get();

	if (loginManager.exists(clientId))
	{
		RequestId request =  loginManager.getCurrentHandler(clientId);

		switch (request)
		{
		case MENU_REQUEST_HANDLER:
			//memory must be deleted
			return new MenuRequestHandler();

		default:
			throw std::runtime_error("No handler found...");
			break;
		}
	}

	//memory must be deleted
	return new LoginRequestHandler();
		 
}



