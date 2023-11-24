#include "Message.hpp"

void	Message::_WHOIS(void)
{
	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
	if (this->_param == "")
	{
		// 431		ERR_NONICKNAMEGIVEN
		this->_response = ":" + this->_server.getServerName() + " 431 :No nickname given\r\n";
		this->_respondUser();
		return ;
	}
	std::map<sockfd_t, User &> &users = this->_server.getUsers();
	std::map<sockfd_t, User &>::iterator it = users.begin();
	for (; it != users.end(); it++)
		if (it->second.getNickname() == this->_param)
			break ;
	if (it == users.end())
	{
		// 401		ERR_NOSUCHNICK
		this->_response = ":" + this->_server.getServerName() + " 401 " + this->_param + " :No such nick/channel\r\n";
		this->_respondUser();
		return ;
	}
	User &user = it->second;
	this->_response = ":" + this->_server.getServerName() + " 311 " + this->_user.getNickname() + " " + user.getNickname() + " " + user.getHostname() + " :" + user.getRealname() + "\r\n";
	this->_respondUser();
	this->_response = ":" + this->_server.getServerName() + " 312 " + this->_user.getNickname() + " " + user.getNickname() + " " + this->_server.getServerName() + " :" + this->_server.getServerName() + ", running version " + this->_server.getVersion() + "\r\n";
	this->_respondUser();
	if (this->_user.checkMode(UMODE_LO))
	{
		this->_response = ":" + this->_server.getServerName() + " 313 " + this->_user.getNickname() + " " + user.getNickname() + " :is an IRC operator\r\n";
		this->_respondUser();
	}
	this->_response = ":" + this->_server.getServerName() + " 315 " + this->_user.getNickname() + " " + user.getNickname() + " :End of WHOIS list\r\n";
	this->_respondUser();
}