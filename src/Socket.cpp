#include "Socket.hpp"
#include <sys/ioctl.h>
#include <fcntl.h>

Socket::Socket(void)
{
    int opt = 1;

	this->_addrLen = sizeof(this->_addr);
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0)
	{
		perror("socket");
		throw std::runtime_error("socket: init fail");
	}
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		throw std::runtime_error("setsockopt: fail");
	}
}

Socket::Socket(int port): _port(port)
{
    int opt = 1;

	this->_addrLen = sizeof(this->_addr);
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0)
	{
		perror("socket");
		throw std::runtime_error("socket: init fail");
	}
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		throw std::runtime_error("setsockopt: fail");
	}
	this->Init(this->_port);
}

Socket::~Socket()
{
	close(this->_fd);
	std::cout << "socket closed" << std::endl;
}

void	Socket::Init(int port)
{
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port);
}

void	Socket::Bind(void)
{
	if (bind(this->_fd, (sockaddr *)&this->_addr, this->_addrLen) < 0)
	{
		perror("bind");
		throw std::runtime_error("bind: fail");
	}
}

int	Socket::Listen(uint n)
{
	int ret = listen(this->_fd, n);
	if (ret > 0)
	{
		perror("listen");
		throw std::runtime_error("listen: fail");
	}
	return (ret);
}

sockfd_t	Socket::Accept(void)
{
	sockfd_t	ret = accept(this->_fd, (sockaddr *)&this->_addr, &this->_addrLen);
	if (ret < 0)
	{
		perror("accept");
		throw std::runtime_error("accept: fail");
	}
	return (ret);
}

void		Socket::Send(const sockfd_t &fd, const std::string &msg)
{
	send(fd, msg.c_str(), msg.length(), 0);
}