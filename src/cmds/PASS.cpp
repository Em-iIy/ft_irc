#include "Message.hpp"

void	Message::_PASS(void)
{
	if (this->_param == "")
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_command + " :Not enough parameters\n";
		this->_respondUser();
		return ;
	}
	if (this->_user.getRegistered())
	{
		// 462		ERR_ALREADYREGISTRED
		this->_response = ":" + this->_server.getServerName() + " 462 :You may not reregister\n";
		this->_respondUser();
		return ;
	}

	this->_user.setPassword(this->_server.checkPassword(this->_param));
	if (!this->_user.getPassword())
	{
		// 464		ERR_PASSWDMISMATCH
		this->_response = ":" + this->_server.getServerName() + " 464 :Password incorrect\n";
	}
	this->_respondUser();
}
