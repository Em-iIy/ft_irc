#pragma once

#include <iostream>
#include <vector>
#include "User.hpp"
#include "Server.hpp"

class Server;
class User;

class Message {
private:
	User							&_user;
	Server							&_server;
	std::string						_command;
	std::string						_param;
	const std::vector<std::string>	_allCommands{"PASS", "NICK", "USER", "QUIT"};
	std::string						_response;

	void	_respondUser(void);
	void	_checkCommand(void);
	void	_PASS(void);
	void	_NICK(void);
	void	_USER(void);
	void	_QUIT(void);

public:
	Message(std::string &msg, User &user, Server &server);
	~Message();

	void	parseMsg(std::string &msg);
};