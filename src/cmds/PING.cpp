#include "Message.hpp"

void	Message::_PING(void)
{
	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
	if (this->_params.size() == 0)
	{
		// 409		ERR_NOORIGIN
		this->_response = ":" + this->_server.getServerName() + " 409 " + this->_user.getNickname() + " :No origin specified\r\n";
		this->_respondUser();
		return ;
	}
	std::string	src;
	std::string	dst;
	std::string rplMsg;

	if (this->_params.size() > 1)
	{
		src = this->_params[0];
		dst = this->_params[1];
		rplMsg = dst + " :" + src;
	}
	else
	{
		dst = this->_params[0];
		rplMsg = dst;
	}
	if (dst != this->_server.getServerName())
	{
		// 402		ERR_NOSUCHSERVER
		this->_response = ":" + this->_server.getServerName() + " 402 " + this->_user.getNickname() + " " + dst + " :No such server\r\n";
		this->_respondUser();
		return ;
	}
	this->_response = ":" + this->_server.getServerName() + " PONG " + rplMsg + "\r\n";
	this->_respondUser();
	return ;
}
