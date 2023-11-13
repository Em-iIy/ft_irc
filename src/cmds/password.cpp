#include "Message.hpp"

void	Message::_pass(void)
{
	if (this->_user.getRegistered())
	{
		// 462		ERR_ALREADYREGISTRED
		this->_response = ":" + this->_server.getConfig().getHostName() + " 462 :You may not reregister\n";
	}
	else
		this->_user.setPassword(this->_server.checkPassword(this->_param));
	if (!this->_user.getPassword())
	{
		// 464		ERR_PASSWDMISMATCH
		this->_response = ":" + this->_server.getConfig().getHostName() + " 464 :Password incorrect\n";
	}
	this->_respondUser();
}
