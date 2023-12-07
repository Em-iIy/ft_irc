#include "Message.hpp"

void	Message::_ISON(void)
{
	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
	if (this->_params.size() == 0)
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_user.getNickname() + " " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}
	// Start response
	this->_response += ":" + this->_server.getServerName() + " 303 " + this->_user.getNickname() + " :";
	for (std::vector<std::string>::iterator it = this->_params.begin(); it != this->_params.end(); ++it)
		if (this->_server.checkNickname(*it))
			this->_response += *it + " ";
	if (this->_response[this->_response.length()] == ' ')
		this->_response.erase(this->_response.length());
	this->_response += "\r\n";
	this->_respondUser();
}
