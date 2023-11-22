#pragma once

#include "Socket.hpp"
#include "Log.hpp"
#include "User.hpp"
#include "Config.hpp"
#include "Message.hpp"
#include "types.hpp"
#include <vector>
#include <map>
#include <poll.h>
#include <iostream>
#include <exception>
#include <algorithm>

class Server {
private:
	Config						_config;
	std::vector<pollfd> 		_pfds;
	std::vector<pollfd> 		_newPfds;
	std::vector<pollfd> 		_dcPfds;
	std::vector<std::string> 	_nicknames;
	std::map<sockfd_t, User &>	_users;
	Socket						_sock;
	int							_port;
	std::string					_password;
	bool						_running;

	Log							_log;

	void						_checkPoll(void);
	void						_pollIn(pollfdIt it);
	void						_pollOut(pollfdIt it);

	bool						_checkDc(int bRead, pollfdIt it);
	void						_disconnectPfds(void);

	void						_acceptConn(void);
	void						_connectPfds(void);

	User						&_getUser(int fd);

public:
	~Server();
	Server(const std::string &port, const std::string &password);
	void	Start(void);
	void	Stop(void);

	void	log(const std::string &src, const std::string &str);

	bool	checkPassword(const std::string &password) const;

	void	disconnectUser(pollfdIt it);

	void	addDcPfd(pollfdIt &it);

	void	broadcastMsg(const std::string &msg);

	bool	checkNickname(const std::string &nickname);
	void	addNickname(const std::string &nickname);
	void	removeNickname(const std::string &nickname);

	const Config		&getConfig(void) const;
	const std::string	&getHostName(void) const;
	const std::string	&getStartDate(void) const;
	const std::string	&getServerName(void) const;
	const std::string	&getVersion(void) const;
	Socket				&getSocket(void);

	std::vector<std::string> 	&getNicknames(void);
	std::map<sockfd_t, User &>	&getUsers(void);
};