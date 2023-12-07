#include "Message.hpp"

void	Message::_PASS(void)
{
	if (this->_params.size() == 0)
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}
	if (this->_user.getRegistered())
	{
		// 462		ERR_ALREADYREGISTRED
		this->_response = ":" + this->_server.getServerName() + " 462 " + this->_user.getNickname() + " :You may not reregister\r\n";
		this->_respondUser();
		return ;
	}

	this->_user.setPassword(this->_server.checkPassword(this->_params[0]));
	if (!this->_user.getPassword())
	{
		// 464		ERR_PASSWDMISMATCH
		this->_response = ":" + this->_server.getServerName() + " 464 :Password incorrect\r\n";
	}
	this->_respondUser();
}
