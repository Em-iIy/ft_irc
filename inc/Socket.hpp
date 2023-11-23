#pragma once

#include "types.hpp"
#include <iostream>
#include <exception>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> // for sockaddr_in
#include <arpa/inet.h>

class Socket {
private:
	sockfd_t	_fd;
	int			_port;
	sockaddr_in	_addr;
	socklen_t	_addrLen;

public:
	Socket();
	~Socket();
	Socket(int port);

	void								Init(int port);
	void								Bind(void);
	int									Listen(uint n);
	std::pair<sockfd_t, sockaddr_in>	Accept(void);
	void								Send(const sockfd_t &fd, const std::string &msg);

	sockfd_t							getFd(void) const {return (this->_fd);};
};