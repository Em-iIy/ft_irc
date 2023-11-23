#include "Message.hpp"

void	Message::_ISON(void)
{
	size_t		space;
	size_t		temp;
	std::string nick;

	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
	if (this->_param == "")
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}
	// Start response
	this->_response += ":" + this->_server.getServerName() + " 303 " + this->_user.getNickname() + " :";
	space = this->_param.find(' ');
	temp = 0;
	// Split all nicknames on the space
	while (space != std::string::npos)
	{
		nick = this->_param.substr(temp, space - temp);
		// Add the nickname to the response if it's in use
		if (this->_server.checkNickname(nick))
			this->_response += nick + " ";
		temp = space + 1;
		space = this->_param.find(' ', temp);
	}
	nick = this->_param.substr(temp, space - temp);
	// Check the last nickname and remove trailing space character from the response
	if (this->_server.checkNickname(nick))
		this->_response += nick;
	else if (this->_response[this->_response.length()] == ' ')
		this->_response.erase(this->_response.length());
	this->_response += "\r\n";
	this->_respondUser();
}