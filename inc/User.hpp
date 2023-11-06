#pragma once

#include <poll.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <exception>

class User {
private:
	pollfd						_fd;
	std::string					_username;
	std::string					_nickname;
	bool						_op = false;
	bool						_registered = false;
	std::vector<std::string> 	_toSend;
	//Channel					&_channel;

public:
	std::string					buffer = "";
	User(pollfd fd);
	~User();
};