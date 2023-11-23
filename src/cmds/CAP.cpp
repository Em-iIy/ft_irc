#include "Message.hpp"

// https://ircv3.net/specs/extensions/capability-negotiation.html
void	Message::_CAP(void)
{
	std::string param;
	size_t	space;

	if (this->_param == "")
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}
	space = this->_param.find(' ');
	if (space != std::string::npos)
		param = this->_param.substr(space + 1);
	else
		param = this->_param;
	// CAP REQ is a valid command but not supported
	if (param == "REQ")
		return ;
	// CAP END terminate capabilities negotiations
	if (param == "END")
	{
		this->_user.capEnd();
		this->_user.checkRegister();
		return ;
	}
	if (param != "LS" &&
		param != "LIST")
	{
		// 410		ERR_INVALIDCAPCMD
		this->_response = ":" + this->_server.getServerName() + " 410 ";
		if (this->_user.getRegistered())
			this->_response += this->_user.getNickname() + " "; 
		this->_response += param + " :Invalid CAP command\r\n";
		this->_respondUser();
		return ;
	}
	// User has started capabilities negotiations
	this->_user.capStart();
	// Respond that no capabilities are supported
	this->_response = "CAP * " + param + "\r\n";
	this->_respondUser();
}