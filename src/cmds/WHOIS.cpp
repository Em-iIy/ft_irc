#include "Message.hpp"

void	Message::_WHOIS(void)
{
	size_t	i;
	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
	if (this->_params.size() == 0)
	{
		// 431		ERR_NONICKNAMEGIVEN
		this->_response = ":" + this->_server.getServerName() + " 431 " + this->_user.getNickname() + " :No nickname given\r\n";
		this->_respondUser();
		return ;
	}
	if (this->_params.size() > 1)
	{
		if (this->_params[0] != this->_server.getServerName())
		{
			// 402		ERR_NOSUCHSERVER
			this->_response = ":" + this->_server.getServerName() + " 402 " + this->_user.getNickname() + " " + this->_params[0] + " :No such server\r\n";
			this->_respondUser();
			return ;
		}
		i = 1;
	}
	else
		i = 0;
	std::string &target = this->_params[i];
	std::map<sockfd_t, User &> &users = this->_server.getUsers();
	std::map<sockfd_t, User &>::iterator it = users.begin();
	for (; it != users.end(); it++)
		if (it->second.getNickname() == target)
			break ;
	if (it == users.end())
	{
		// 401		ERR_NOSUCHNICK
		this->_response = ":" + this->_server.getServerName() + " 401 " + this->_user.getNickname() + " " + target + " :No such nick/channel\r\n";
		this->_respondUser();
		return ;
	}
	User &user = it->second;
	// Check whether user is invisible
	if (user.checkMode(UMODE_I) == false) // Add "||  this->_user and user share a channel
	{
		this->_response = ":" + this->_server.getServerName() + " 311 " + this->_user.getNickname() + " " + user.getNickname() + " " + user.getHostname() + " :" + user.getRealname() + "\r\n";
		this->_respondUser();
		this->_response = ":" + this->_server.getServerName() + " 312 " + this->_user.getNickname() + " " + user.getNickname() + " " + this->_server.getServerName() + " :" + this->_server.getServerName() + ", running version " + this->_server.getVersion() + "\r\n";
		this->_respondUser();
		if (this->_user.checkMode(UMODE_LO))
		{
			this->_response = ":" + this->_server.getServerName() + " 313 " + this->_user.getNickname() + " " + user.getNickname() + " :is an IRC operator\r\n";
			this->_respondUser();
		}
	}
	this->_response = ":" + this->_server.getServerName() + " 315 " + this->_user.getNickname() + " " + user.getNickname() + " :End of WHOIS list\r\n";
	this->_respondUser();
}
