#include "User.hpp"

User::User(pollfd fd): _fd(fd)
{
	std::cout << "User #" << this->_fd.fd << " Connected." << std::endl;
	this->resetBuffer();
}

User::~User()
{
	std::cout << "User #" << this->_fd.fd << " Disconnected." << std::endl;
	close(this->_fd.fd);
}

void	User::resetBuffer(void)
{
	this->buffer = std::to_string(this->_fd.fd) + ": ";
}

void	User::appendBuffer(std::string msg)
{
	this->buffer += msg;
}

int User::getFd(void) const
{
	return (this->_fd.fd);
}

const std::string	&User::getUsername(void) const
{
	return (this->_username);
}

const std::string	&User::getNickname(void) const
{
	return (this->_nickname);
}