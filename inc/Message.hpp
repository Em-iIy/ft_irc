#pragma once

#include "User.hpp"
#include "Server.hpp"
#include "types.hpp"
#include "utils.hpp"
#include <iostream>
#include <vector>

class Message {
private:
	User							&_user;
	Server							&_server;
	pollfdIt						_it;
	std::string						_command;
	std::string						_param;
	// const std::vector<std::string>	_allCommands{"PING", "MODE", "AWAY", "NICK", "USER", "PASS", "CAP", "QUIT"};
	std::string						_response;

	void	_respondUser(void);
	cmd_e	_checkCommand(void);
	void	_runCommand(void);

	void	_USER_MODE(std::string &target, std::string &mode);
	void	_CHANNEL_MODE(std::string &target, std::string &mode);

	void	_AWAY(void);
	void	_CAP(void);
	void	_MODE(void);
	void	_NICK(void);
	void	_PASS(void);
	void	_PING(void);
	void	_QUIT(void);
	void	_USER(void);
	void	_VERSION(void);

public:
	Message(std::string &msg, User &user, pollfdIt &it, Server &server);
	~Message();

	void	parseMsg(std::string &msg);
};