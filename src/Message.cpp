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
	this->_runCommand();
}

void	Message::_respondUser(void)
{
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
	default:
		break;
	}
}
