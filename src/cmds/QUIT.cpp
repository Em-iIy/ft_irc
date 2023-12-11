#include "Message.hpp"

void	Message::_QUIT(void)
{
	// Create a response if the user was fully registered
	if (this->_user.getRegistered())
	{
		std::list<Channel *>	&channels = this->_user.getChannels();
		std::string				msg;
		std::string				prefix = ":" + this->_user.getFullRef() + " QUIT ";
		// Add the default quit message or the message from the parameters if provided
		if (this->_params.size() != 0)
			msg = " :" + this->_params[0] + "\r\n";
		else
			msg = " :Client Quit\r\n";
		for (std::list<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			this->_response = prefix + (*it)->getName() + msg;
			this->_respondChannel(*it);
		}
	}
	// Set the user for disconnection
	this->_server.addDcPfd(this->_it);
}
