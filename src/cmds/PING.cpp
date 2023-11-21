#include "Message.hpp"

void	Message::_PING(void)
{
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
	if (dst != this->_server.getHostName())
	{
		this->_response = ":" + this->_server.getHostName() + " 402 " + dst + " :No such server\n";
		this->_respondUser();
		return ;
	}
	this->_response = ":" + this->_server.getHostName() + " PONG " + this->_server.getServerName() + " :" + msg + "\n";
	this->_respondUser();
	return ;
}