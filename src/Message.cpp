#include "Message.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
#include <exception>

// removes trailing \r\n from strings
static void	rmCRLF(std::string &str)
{
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.cend());
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.cend());
}

Message::Message(std::string &msg, User &user, Server &server) : _user(user), _server(server)
{
	this->parseMsg(msg);
}

Message::~Message(void)
{
}

void	Message::parseMsg(std::string &msg)
{
	// Remove \r\n from command string
	rmCRLF(msg);
	std::stringstream			input(msg);
	std::string					token;

	// Reset buffer before potentially deleting the user
	this->_user.resetBuffer();
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
		this->_response = ":" + this->_server.getConfig().getHostName() + " 421 " + this->_command + " :Unknown command\n";
		this->_respondUser();
		return ;
	}
	if (*it == "QUIT")
	{
		this->_quit();
		return ;
	}
	else if (this->_param == "") // Commands after this require parameters
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getConfig().getHostName() + " 461 " + this->_command + " :Not enough parameters\n";
		this->_respondUser();
		return ;
	}
	else if (*it == "PASS")
	{
		this->_pass();
		return ;
	}
}

void	Message::_pass(void)
{
	if (this->_user.getRegistered())
	{
		// 462		ERR_ALREADYREGISTRED
		this->_response = ":" + this->_server.getConfig().getHostName() + " 462 :You may not reregister\n";
	}
	else
		this->_user.setPassword(this->_server.checkPassword(this->_param));
	if (!this->_user.getPassword())
	{
		// 464		ERR_PASSWDMISMATCH
		this->_response = ":" + this->_server.getConfig().getHostName() + " 464 :Password incorrect\n";
	}
	this->_respondUser();
}

void	Message::_quit(void)
{
	// Create a response if the user was fully registered
	if (this->_user.getRegistered())
	{
		this->_response = ":" + this->_user.getFullRef() + " QUIT :";
		// Add the default quit message or the message from the parameters if provided
		if (this->_param != "")
			this->_response += this->_param;
		else
			this->_response += "Client Quit";
		this->_response += "\n";
		this->_respondUser(); // should be all channels + all users who have been messaged
	}
	// Disconnect the user from the server
	this->_server.disconnectUser(this->_user.getIdx());
}