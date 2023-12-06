#pragma once

#include <iostream>
#include <list>
#include <clocale>
#include "types.hpp"

class Channel{
private:
	std::string				_name;
	std::string				_pass;
	std::string				_topic = "";
	std::list<User *>		_users;
	std::list<User *>		_opers;
	cmode_t					_mode = CMODE_NONE;

public:
	Channel(std::string &name, std::string &pass, User *creator);
	~Channel();

	const std::string	&getName(void);
	const std::string	&getPass(void);
	const std::string	&getTopic(void);
	std::list<User *>	&getUsers(void);
	std::list<User *>	&getOpers(void);
	cmode_t				&getMode(void);
	bool				checkMode(cmode_t mode) const;

	void	addMode(cmode_t mode);
	void	rmMode(cmode_t mode);
	void	addUser(User *user, std::string &pass);
	void	rmUser(User *user);
	void	addOper(User *user);
	void	rmOper(User *user);
	bool	isUser(User *user);
	bool	isOper(User *user);
};

std::ostream	&operator<<(std::ostream &out, Channel &c);