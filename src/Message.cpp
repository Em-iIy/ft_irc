#include "Message.hpp"
#include "utils.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
#include <exception>


Message::Message(std::string &msg, User &user, pollfdIt &it, Server &server) : _user(user), _server(server), _it(it)
{
	// Reserve the max amount of arguments a command may have
	this->_params.reserve(ARG_COUNT);
	// Separate the message into <command> [(vector<string>[14])<params>] [:<trailing>]
	this->parseMsg(msg);
	// Check and execute the command
	this->_runCommand();
	
	if (user.getRegistered())
		server.log(user.getNickname(), msg);
	else
		server.log("User #" + std::to_string(user.getFd()), msg);
}

Message::~Message(void)
{
}

// Splits input message into <command> [(vector<string>[14])<params>] [:<trailing>]
// Still splits message into <command> [param] until all commands are updated to ^
void	Message::parseMsg(std::string &msg)
{
	// Remove \r\n from command string
	rmCRLF(msg);
	std::string	temp;

	// Remove when commands are updated
	{
		std::stringstream			input(msg);
		std::getline(input, temp, ' ');
		std::getline(input, this->_param);
	}

	size_t colon = msg.find(" :");
	size_t	space = 0;
	size_t	arg = 0;

	space = msg.find(' ', space);
	// Return in case of command without arguments
	if (space == std::string::npos)
	{
		this->_command = msg;
		return ;
	}
	// Separate out the command
	this->_command = msg.substr(arg, space - arg);
	// Move arg to 1 past the space and find next space
	arg = space + 1;
	space = msg.find(' ', arg);
	while (space != std::string::npos && this->_params.size() < 14)
	{
		// Separate out the trailing argument after the ':'
		if (msg[arg] == ':')
		{
			this->_trailing = msg.substr(arg + 1);
			return ;
		}
		// Separate out the argument
		if (space != arg) // otherwise it's an empty string
		{
			temp = msg.substr(arg, space - arg);
			this->_params.push_back(temp);
		}
		// Move arg to 1 past the space and find next space
		arg = space + 1;
		space = msg.find(' ', arg);
	}
	// Separate out the trailing argument after the ':'
	if (this->_params.size() == 14)
	{
		colon = msg.find(" :", arg);
		if (colon != std::string::npos)
			this->_trailing = msg.substr(colon + 2);
		return ;
	}
	if (msg[arg] == ':')
	{
		this->_trailing = msg.substr(arg + 1);
		return ;
	}
	// Get the last parameter
	if (arg != msg.length()) // otherwise it's an empty string
	{
		temp = msg.substr(arg);
		this->_params.push_back(temp);
	}
}

void	Message::_respondUser(void)
{
	msgLimitSize(this->_response);
	this->_user.toSend.push_back(this->_response);
}

cmd_e	Message::_checkCommand(void)
{
	if (this->_command == "PING")
		return (CMD_PING);
	if (this->_command == "MODE")
		return (CMD_MODE);
	if (this->_command == "AWAY")
		return (CMD_AWAY);
	if (this->_command == "NICK")
		return (CMD_NICK);
	if (this->_command == "WHOIS")
		return (CMD_WHOIS);
	if (this->_command == "VERSION")
		return (CMD_VERSION);
	if (this->_command == "ISON")
		return (CMD_ISON);
	if (this->_command == "USER")
		return (CMD_USER);
	if (this->_command == "PASS")
		return (CMD_PASS);
	if (this->_command == "CAP")
		return (CMD_CAP);
	if (this->_command == "QUIT")
		return (CMD_QUIT);
	return (CMD_NOT_FOUND);
}

void	Message::_runCommand(void)
{
	switch (this->_checkCommand())
	{
	case CMD_NOT_FOUND:
		this->_response = ":" + this->_server.getServerName() + " 421 " + this->_command + " :Unknown command\n";
		this->_respondUser();
		break ;
	case CMD_AWAY:
		this->_AWAY();
		break ;
	case CMD_CAP:
		this->_CAP();
		break ;
	case CMD_ISON:
		this->_ISON();
		break ;
	case CMD_MODE:
		this->_MODE();
		break ;
	case CMD_NICK:
		this->_NICK();
		break ;
	case CMD_PASS:
		this->_PASS();
		break ;
	case CMD_PING:
		this->_PING();
		break ;
	case CMD_QUIT:
		this->_QUIT();
		break ;
	case CMD_USER:
		this->_USER();
		break ;
	case CMD_VERSION:
		this->_VERSION();
		break ;
	case CMD_WHOIS:
		this->_WHOIS();
		break ;
	default:
		break;
	}
}
