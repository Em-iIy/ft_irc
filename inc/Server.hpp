#pragma once

#include <vector>
#include <map>
#include <Server.hpp>
#include <Socket.hpp>
#include <poll.h>
#include <iostream>
#include <exception>
#include "Socket.hpp"
#include "User.hpp"
#include "Config.hpp"
#include "Message.hpp"

class User;

class Server {
private:
	Config						_config;
	std::vector<pollfd> 		_pfds;
	std::map<sockfd_t, User &>	_users;
	Socket						_sock;
	int							_port;
	std::string					_password;

	void						_checkPoll(void);
	void						_pollIn(int i);
	void						_pollOut(int i);

	bool						_checkDc(int bRead, int i);
	void						_acceptConn(void);

	void						_relayMsg(std::string &msg, int i);

	User						&_getUser(int i);

public:
	~Server();
	Server(const std::string &port, const std::string &password);
	void	Start(void);

	bool	checkPassword(const std::string &password) const;
	void	disconnectUser(int i);

	Config	&getConfig(void);
	Socket	&getSocket(void);
};