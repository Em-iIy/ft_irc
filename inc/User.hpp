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
	//Channel					&_channel;

public:
	std::string					buffer;
	std::vector<std::string> 	toSend;
	User(pollfd fd);
	~User();
	void	resetBuffer(void);
	void	appendBuffer(std::string msg);
	int	getFd(void) const {return (this->_fd.fd);};
};