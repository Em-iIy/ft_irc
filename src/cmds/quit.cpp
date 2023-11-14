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
		// this->_server.relayMsg(this->_response, 0);
		this->_respondUser(); // should be all channels + all users who have been messaged
	}
	// Disconnect the user from the server
	this->_server.disconnectUserFd(this->_user.getFd());
}
