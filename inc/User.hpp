#pragma once

#include "Server.hpp"
#include "types.hpp"
#include "Channel.hpp"
#include <poll.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <exception>
#include <list>
#include <ctime>

class User {
private:
	Server						&_serv;
	pollfd						_fd;
	sockaddr_in					_sock;
	std::string					_nickname;
	std::string					_username;
	std::string					_hostname;
	std::string					_servername;
	std::string					_realname;
	std::string					_fullRef;
	std::string					_awayMsg;
	status_t					_status = STAT_NEW;
	umode_t						_mode = UMODE_NONE;
	std::time_t					_idleTime;
	std::list<Channel *>		_channels;

public:
	std::string					buffer;
	std::vector<std::string> 	toSend;

	User(Server &serv, pollfd fd, sockaddr_in sock);
	~User();
	void	resetBuffer(void);
	void	appendBuffer(std::string msg);
	void	updateFullRef(void);
	void	registerUser(std::string &username, std::string &hostname, std::string &servername, std::string &realname);
	void	checkRegister(void);


	const int			&getFd(void) const;
	std::string			getIP(void) const;
	int					getPort(void) const;
	const std::string	&getUsername(void) const;
	const std::string	&getNickname(void) const;
	const std::string	&getHostname(void) const;
	const std::string	&getServername(void) const;
	const std::string	&getRealname(void) const;
	const std::string	&getFullRef(void) const;
	const std::string	&getAwayMsg(void) const;
	bool				getRegistered(void) const;
	bool				getPassword(void) const;
	const umode_t		&getMode(void) const;
	bool				checkMode(umode_t mode) const;
	std::time_t			getIdle(void) const;
	std::list<Channel *>	&getChannels(void) {return (this->_channels);};

	void				setUsername(const std::string &username);
	void				setNickname(const std::string &nickname);
	void				setAwayMsg(const std::string &msg);
	void				setRegistered(const bool &registered);
	void				setPassword(const bool &password);
	void				addMode(umode_t mode);
	void				rmMode(umode_t mode);
	void				setIdle(const std::time_t &time);
	void				addToChannel(Channel *channel);
	void				rmFromChannel(Channel *channel);

	void				capStart(void);
	void				capEnd(void);
};

std::ostream	&operator<<(std::ostream &o, User &rhs);