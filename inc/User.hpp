#pragma once

#include <poll.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <exception>
#include "Server.hpp"

class Server;

class User {
private:
	Server						&_serv;
	pollfd						_fd;
	int							_idx;
	std::string					_username;
	std::string					_nickname;
	std::string					_fullRef;
	bool						_op = false;
	bool						_registered = false;
	bool						_password = false;
	//Channel					&_channel;

public:
	std::string					buffer;
	std::vector<std::string> 	toSend;

	User(Server &serv, pollfd fd, int i);
	~User();
	void	resetBuffer(void);
	void	appendBuffer(std::string msg);
	void	updateFullRef(void);

	int					getFd(void) const;
	int					getIdx(void) const;
	const std::string	&getUsername(void) const;
	const std::string	&getNickname(void) const;
	const std::string	&getFullRef(void) const;
	const bool			&getRegistered(void) const;
	const bool			&getPassword(void) const;

	void				setUsername(const std::string &username);
	void				setNickname(const std::string &nickname);
	void				setRegistered(const bool &registered);
	void				setPassword(const bool &password);

};