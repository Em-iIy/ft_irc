#pragma once

#include <poll.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <exception>
#include "Server.hpp"
#include "types.hpp"

class User {
private:
	Server						&_serv;
	pollfd						_fd;
	std::string					_nickname;
	std::string					_username;
	std::string					_hostname;
	std::string					_servername;
	std::string					_realname;
	std::string					_fullRef;
	status_t					_status = STAT_NEW;
	//Channel					&_channel;

public:
	std::string					buffer;
	std::vector<std::string> 	toSend;

	User(Server &serv, pollfd fd);
	~User();
	void	resetBuffer(void);
	void	appendBuffer(std::string msg);
	void	updateFullRef(void);
	void	registerUser(std::string &username, std::string &hostname, std::string &servername, std::string &realname);
	void	checkRegister(void);

	int					getFd(void) const;

	const std::string	&getUsername(void) const;
	const std::string	&getNickname(void) const;
	const std::string	&getHostname(void) const;
	const std::string	&getServername(void) const;
	const std::string	&getRealname(void) const;
	const std::string	&getFullRef(void) const;
	const bool			getRegistered(void) const;
	const bool			getPassword(void) const;

	void				setUsername(const std::string &username);
	void				setNickname(const std::string &nickname);
	void				setRegistered(const bool &registered);
	void				setPassword(const bool &password);

	void				capStart(void);
	void				capEnd(void);
};

std::ostream	&operator<<(std::ostream &o, const User &rhs);