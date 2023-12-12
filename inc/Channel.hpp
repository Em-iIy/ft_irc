#pragma once

#include <iostream>
#include <list>
#include <clocale>
#include <ctime>
#include "types.hpp"

class Channel{
private:
	Server					&_server;
	std::string				_name;
	std::string				_pass;
	std::string				_topic = "";
	std::list<User *>		_users;
	std::list<User *>		_opers;
	std::list<User *>		_whitelist;
	cmode_t					_mode = CMODE_NONE;

public:
	Channel(std::string &name, std::string &pass, User *creator, Server &server);
	~Channel();

	const std::string	&getName(void);
	const std::string	&getPass(void);
	const std::string	&getTopic(void);
	std::list<User *>	&getUsers(void);
	std::list<User *>	&getOpers(void);
	cmode_t				&getMode(void);
	int					getLimit(void);
	bool				checkMode(cmode_t mode) const;
	User				*getUserFromNick(std::string &nick);

	void				setTopic(const std::string &topic);

	void	addMode(cmode_t mode);
	void	rmMode(cmode_t mode);
	void	addUser(User *user, std::string &pass);
	bool	rmUser(User *user);
	void	addOper(User *user);
	void	rmOper(User *user);
	void	makeWhitelist(void);
	void	addWhitelist(User *user);
	void	rmWhitelist(User *user);
	void	setPass(std::string pass);
	void	setLimit(int limit);
	void	setMode(cmode_t &mode);
	void	setOpers(std::list<User *> &opers);
	bool	isUser(User *user);
	bool	isOper(User *user);
	bool	isWhitelisted(User *user);
};

std::ostream	&operator<<(std::ostream &out, Channel &c);