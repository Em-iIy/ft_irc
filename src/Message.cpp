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
	std::vector<std::string>::const_iterator	it =
		std::find(this->_allCommands.begin(), this->_allCommands.end(), this->_command);
	
}