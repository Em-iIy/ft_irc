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
	std::string						_response;

	bool	_JOIN0();
	void	_welcomeChannel(Channel *channel);

	void	_respondUser(void);
	void	_respondTargetUser(User *user);
	void	_respondChannel(Channel *channel);
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
	void	_PRIVMSG(void);
	void	_QUIT(void);
	void	_TOPIC(void);
	void	_USER(void);
	void	_VERSION(void);
	void	_WHOIS(void);
	void	_JOIN(void);
	void	_PART(void);
	void	_INVITE(void);
	void	_KICK(void);
	void	_WHO(void);

	void	_singleKICK(std::string &channelName, std::string &userNick, std::string &comment);
	void	_makeWHOResponse(User *user);

public:
	Message(std::string &msg, User &user, pollfdIt &it, Server &server);
	~Message();

	void	parseMsg(std::string &msg);
};
