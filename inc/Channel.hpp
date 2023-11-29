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

	bool	_userInChannel(User *user);

public:
	Channel(std::string &name, std::string &pass, User *creator);
	~Channel();

	const std::string	&getName(void);
	const std::string	&getPass(void);

	void	addMode(cmode_t mode);
	void	rmMode(cmode_t mode);
	void	addUser(User *user, std::string &pass);
};