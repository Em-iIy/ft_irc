#include "Message.hpp"

// https://ircv3.net/specs/extensions/capability-negotiation.html
void	Message::_CAP(void)
{
	if (this->_params.size() == 0)
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}
	std::string &param = this->_params[0];
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