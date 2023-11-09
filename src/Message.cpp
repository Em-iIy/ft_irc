#include "Message.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
#include <exception>

Message::Message(std::string &msg, User &user, Server &server) : _user(user), _server(server)
{
	this->parseMsg(msg);
}

Message::~Message(void)
{
}

void	Message::parseMsg(std::string &msg)
{
	std::stringstream			input(msg);
	std::string					token;

	std::getline(input, this->_command, ' ');
	std::getline(input, this->_param);
	this->_checkCommand();
}

void	Message::_checkCommand(void)
{
	if (this->_param == "")
	{
		// 461 ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getConfig().getHostName() + " 461 " + this->_command + " :Not enough parameters\n";
	}
	std::vector<std::string>::const_iterator	it = std::find(this->_allCommands.begin(), this->_allCommands.end(), this->_command);
	if (it == this->_allCommands.end())
	{
		// 421 ERR_UNKNOWNCOMMAND
		this->_response = ":" + this->_server.getConfig().getHostName() + " 421 " + this->_command + " :Unknown command\n";
	}
}