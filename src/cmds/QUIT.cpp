#include "Message.hpp"

void	Message::_QUIT(void)
{
	// Create a response if the user was fully registered
	if (this->_user.getRegistered())
	{
		this->_response = ":" + this->_user.getFullRef() + " QUIT :";
		// Add the default quit message or the message from the parameters if provided
		if (this->_param != "")
			this->_response += this->_param;
		else
			this->_response += "Client Quit";
		this->_response += "\n";
		this->_server.broadcastMsg(this->_response);
	}
	// Set the user for disconnection
	this->_server.addDcPfd(this->_it);
}
