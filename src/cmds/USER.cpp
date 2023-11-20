#include "Message.hpp"
#include <sstream>

void	Message::_USER(void)
{
	std::stringstream	param(this->_param);
	std::string	username;
	std::string	hostname;
	std::string	servername;
	std::string	realname;

	std::getline(param, username, ' ');
	std::getline(param, hostname, ' ');
	std::getline(param, servername, ' ');
	std::getline(param, realname, '\0');

	if (username.length() == 0 || 
		hostname.length() == 0 || 
		servername.length() == 0 || 
		realname.length() <= 1 ||
		realname[0] != ':'
		)
	{
		this->_response = ":" + this->_server.getConfig().getHostName() + " 461 " + this->_command + " :Not enough parameters\n";
		this->_respondUser();
		return ;
	}
	// USER cannot be used to change already registered details
	if (this->_user.getRegistered())
	{
		// 462		ERR_ALREADYREGISTRED
		this->_response = ":" + this->_server.getConfig().getHostName() + " 462 :You may not reregister\n";
		this->_respondUser();
		return ;
	}
	// remove the colon from the start of real name
	realname.erase(0, 1);
	this->_user.registerUser(username, hostname, servername, realname);
	this->_user.checkRegister();
}