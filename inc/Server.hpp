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

class Server {
private:
	std::vector<pollfd> 		_pfds;
	std::map<sockfd_t, User &>	_users;
	Socket						_sock;
	std::string					_password;
	int							_port;

	void						_checkPoll(void);
	void						_pollIn(int i);
	void						_acceptConn(void);
	bool						_checkDc(int i);
	void						_relayMsg(std::string &msg, int i);
	User						&_getUser(int i);

public:
	~Server();
	Server(const std::string &port, const std::string &password);
	void	Start(void);
};