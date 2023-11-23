#include "Message.hpp"

void	Message::_VERSION(void)
{
	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
	if (this->_param != "" && this->_param != this->_server.getServerName())
	{
		// 402		ERR_NOSUCHSERVER
		this->_response = ":" + this->_server.getServerName() + " 402 " + this->_param + " :No such server\n";
		this->_respondUser();
		return ;
	}
	this->_response = ":" + this->_server.getServerName() + " 351 " + this->_user.getNickname() + " " + this->_server.getVersion() + "." + this->_server.getDebug() + " " + this->_server.getServerName() + " :" + this->_server.getComments() + "\n";
	this->_respondUser();
}