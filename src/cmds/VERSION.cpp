#include "Message.hpp"

void	Message::_VERSION(void)
{
	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
	if (this->_params.size() != 0 && this->_params[0] != this->_server.getServerName())
	{
		// 402		ERR_NOSUCHSERVER
		this->_response = ":" + this->_server.getServerName() + " 402 " + this->_params[0] + " :No such server\r\n";
		this->_respondUser();
		return ;
	}
	this->_response = ":" + this->_server.getServerName() + " 351 " + this->_user.getNickname() + " " + this->_server.getVersion() + "." + this->_server.getDebug() + " " + this->_server.getServerName() + " :" + this->_server.getComments() + "\r\n";
	this->_respondUser();
}