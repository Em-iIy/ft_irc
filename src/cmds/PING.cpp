#include "Message.hpp"

void	Message::_PING(void)
{
	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
	if (this->_param == "")
	{
		// 409		ERR_NOORIGIN
		this->_response = ":" + this->_server.getServerName() + " 409 :No origin specified\r\n";
		this->_respondUser();
		return ;
	}
		
	std::string	src;
	std::string	dst;
	std::string rplMsg;
	size_t		space;

	space = this->_param.find(' ');
	if (space != std::string::npos)
	{
		src = this->_param.substr(0, space);
		dst = this->_param.substr(space + 1);
		rplMsg = dst + " :" + src;
	}
	else
	{
		src = "";
		dst = this->_param;
		rplMsg = dst;
	}
	if (dst != this->_server.getServerName())
	{
		// 402		ERR_NOSUCHSERVER
		this->_response = ":" + this->_server.getServerName() + " 402 " + dst + " :No such server\r\n";
		this->_respondUser();
		return ;
	}
	this->_response = ":" + this->_server.getServerName() + " PONG " + rplMsg + "\r\n";
	this->_respondUser();
	return ;
}