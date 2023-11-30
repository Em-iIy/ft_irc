#pragma once

#include "User.hpp"
#include "Server.hpp"
#include "types.hpp"
#include "utils.hpp"
#include <iostream>
#include <vector>

#define ARG_COUNT 14

class Message {
private:
	User							&_user;
	Server							&_server;
	pollfdIt						_it;
	std::string						_command;
	std::vector<std::string>		_params;
	std::string						_trailing;
	std::string						_param;
	std::string						_response;

	void	_respondUser(void);
	cmd_e	_checkCommand(void);
	void	_runCommand(void);

	void	_USER_MODE(std::string &target, std::string &mode);
	void	_CHANNEL_MODE(std::string &target, std::string &mode);

	void	_AWAY(void);
	void	_CAP(void);
	void	_ISON(void);
	void	_MODE(void);
	void	_NICK(void);
	void	_PASS(void);
	void	_PING(void);
	void	_PONG(void);
	void	_QUIT(void);
	void	_USER(void);
	void	_VERSION(void);
	void	_WHOIS(void);
	void	_JOIN(void);
	void	_PART(void);

public:
	Message(std::string &msg, User &user, pollfdIt &it, Server &server);
	~Message();

	void	parseMsg(std::string &msg);
};