#pragma once

#include <iostream>
#include <vector>
#include "User.hpp"
#include "Server.hpp"
#include "types.hpp"

class Message {
private:
	User							&_user;
	Server							&_server;
	pollfdIt						_it;
	std::string						_command;
	std::string						_param;
	const std::vector<std::string>	_allCommands{"PASS", "NICK", "USER", "QUIT", "PING"};
	std::string						_response;

	void	_respondUser(void);
	void	_checkCommand(void);
	void	_PASS(void);
	void	_NICK(void);
	void	_USER(void);
	void	_QUIT(void);

public:
	Message(std::string &msg, User &user, pollfdIt &it, Server &server);
	~Message();

	void	parseMsg(std::string &msg);
};