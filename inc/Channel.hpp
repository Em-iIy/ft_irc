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
	umode_t					_mode = UMODE_NONE;

public:
	Channel();
	~Channel();
};