#include "Message.hpp"
#include "utils.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
#include <exception>


Message::Message(std::string &msg, User &user, pollfdIt &it, Server &server) : _user(user), _it(it), _server(server)
{
	this->parseMsg(msg);
	
	if (user.getRegistered())
		server.log(user.getNickname(), msg);
	else
		server.log("User #" + std::to_string(user.getFd()), msg);
}

Message::~Message(void)
{
}

void	Message::parseMsg(std::string &msg)
{
	// Remove \r\n from command string
	rmCRLF(msg);
	std::stringstream			input(msg);

	// Separate the message into <command> [<parameters>]
	std::getline(input, this->_command, ' ');
	std::getline(input, this->_param);
	// Check and execute the command
	this->_checkCommand();
}

void	Message::_respondUser(void)
{
	this->_user.toSend.push_back(this->_response);
}

void	Message::_checkCommand(void)
{
	std::vector<std::string>::const_iterator	it = std::find(this->_allCommands.begin(), this->_allCommands.end(), this->_command);

	if (it == this->_allCommands.end()) // Check if the command was found
	{
		// 421		ERR_UNKNOWNCOMMAND
		this->_response = ":" + this->_server.getHostName() + " 421 " + this->_command + " :Unknown command\n";
		this->_respondUser();
		return ;
	}
	if (*it == "CAP")
	{
		this->_CAP();
		return ;
	}
	if (*it == "QUIT")
	{
		this->_QUIT();
		return ;
	}
	if (*it == "PASS")
	{
		this->_PASS();
		return ;
	}
	// Must have a correct password to use any other commands
	if (this->_user.getPassword() == false)
		return ;
	if (*it == "NICK")
	{
		this->_NICK();
		return ;
	}
	if (*it == "USER")
	{
		this->_USER();
		return ;
	}
	// Must be registered to use any other commands
	if (this->_user.getRegistered() == false)
		return ;
	if (*it == "PING")
	{
		this->_PING();
		return ;
	}
	if (*it == "MODE")
	{
		this->_MODE();
		return ;
	}
	if (*it == "AWAY")
	{
		this->_AWAY();
		return ;
	}
}
