#include "Message.hpp"

void	Message::_PING(void)
{
	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
		
	std::string	dst;
	std::string	msg;
	size_t		colon;
	size_t		space;

	space = this->_param.find(' ');
	colon = this->_param.find(':');
	if (space != std::string::npos && colon != std::string::npos)
	{
		dst = this->_param.substr(0, space);
		msg = this->_param.substr(colon + 1);
	}
	else
	{
		dst = this->_param;
	}
	if (dst != this->_server.getServerName())
	{
		// 402		ERR_NOSUCHSERVER
		this->_response = ":" + this->_server.getServerName() + " 402 " + dst + " :No such server\r\n";
		this->_respondUser();
		return ;
	}
	this->_response = ":" + this->_server.getServerName() + " PONG " + this->_server.getServerName() + " :" + msg + "\r\n";
	this->_respondUser();
	return ;
}