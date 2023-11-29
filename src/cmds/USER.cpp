#include "Message.hpp"
#include <sstream>

void	Message::_USER(void)
{
	if (this->_user.getPassword() == false)
		return ;
	if (this->_params.size() < 4)
	{
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}
	// USER cannot be used to change already registered details
	if (this->_user.getRegistered())
	{
		// 462		ERR_ALREADYREGISTRED
		this->_response = ":" + this->_server.getServerName() + " 462 :You may not reregister\r\n";
		this->_respondUser();
		return ;
	}
	this->_user.registerUser(this->_params[0], this->_params[1], this->_params[2], this->_params[3]);
	this->_user.checkRegister();
}