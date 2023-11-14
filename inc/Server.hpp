#pragma once

#include <vector>
#include <map>
#include <poll.h>
#include <iostream>
#include <exception>
#include <algorithm>
#include "Socket.hpp"
#include "User.hpp"
#include "Config.hpp"
#include "Message.hpp"

#define DEBUG(x) std::cout << "User #" << x << " " << __func__ << " " << __FILE__ << ":" << __LINE__ << std::endl

class User;

class Server {
private:
	Config						_config;
	std::vector<pollfd> 		_pfds;
	std::vector<std::string> 	_nicknames;
	std::map<sockfd_t, User &>	_users;
	Socket						_sock;
	int							_port;
	std::string					_password;

	void						_checkPoll(void);
	void						_pollIn(int i);
	void						_pollOut(int i);

	bool						_checkDc(int bRead, int i);
	void						_acceptConn(void);


	User						&_getUser(int i);

public:
	~Server();
	Server(const std::string &port, const std::string &password);
	void	Start(void);

	bool	checkPassword(const std::string &password) const;
	void	disconnectUser(int i);

	void		relayMsg(std::string &msg, int i);

	bool		checkNickname(const std::string nickname);
	void		addNickname(const std::string &nickname);
	void		removeNickname(const std::string &nickname);

	Config	&getConfig(void);
	Socket	&getSocket(void);
};