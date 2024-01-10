#include "Socket.hpp"

Socket::Socket(void)
{
    int opt = 1;

	this->_addrLen = sizeof(this->_addr);
	// Create new socket file descriptor
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0)
	{
		perror("socket");
		throw std::runtime_error("socket: init fail");
	}
	// Set the socket options
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
	// Create new socket file descriptor
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0)
	{
		perror("socket");
		throw std::runtime_error("socket: init fail");
	}
	// Set the socket options
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		throw std::runtime_error("setsockopt: fail");
	}
	this->Init(this->_port);
}

Socket::~Socket()
{
	// Close the socket file descriptor
	close(this->_fd);
	std::cout << "socket closed" << std::endl;
}

void	Socket::Init(int port)
{
	// Configures the socket address settings
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port);
}

void	Socket::Bind(void)
{
	// Binds the socket address to the socket file descriptor
	if (bind(this->_fd, (sockaddr *)&this->_addr, this->_addrLen) < 0)
	{
		perror("bind");
		throw std::runtime_error("bind: fail");
	}
}

int	Socket::Listen(uint n)
{
	// Listens for new cnnections on the socket
	int ret = listen(this->_fd, n);
	if (ret > 0)
	{
		perror("listen");
		throw std::runtime_error("listen: fail");
	}
	return (ret);
}

std::pair<sockfd_t, sockaddr_in>	Socket::Accept(void)
{
	std::pair<sockfd_t, sockaddr_in> ret;
	// Accepts new connection on the socket
	ret.first = accept(this->_fd, (sockaddr *)&this->_addr, &this->_addrLen);
	if (ret.first < 0)
	{
		perror("accept");
		throw std::runtime_error("accept: fail");
	}
	ret.second = this->_addr;
	std::cout << inet_ntoa(this->_addr.sin_addr) << ":" << this->_addr.sin_port << std::endl;
	return (ret);
}

void		Socket::Send(const sockfd_t &fd, const std::string &msg)
{
	// Send msg to the given file descriptor
	ssize_t bSent = send(fd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
	if (bSent < 0)
	{
		perror("send");
		throw std::runtime_error("User #" + std::to_string(fd) + " closed socket before message could be sent");
	}
}
