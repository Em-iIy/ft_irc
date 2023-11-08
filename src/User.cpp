#include "User.hpp"

User::User(Server &serv, pollfd fd): _serv(serv), _fd(fd)
{
	std::cout << "User #" << this->_fd.fd << " Connected." << std::endl;
	this->resetBuffer();
	this->_username = "";
	this->_nickname = "";
}

User::~User()
{
	std::cout << "User #" << this->_fd.fd << " Disconnected." << std::endl;
	close(this->_fd.fd);
}

void	User::resetBuffer(void)
{
	// this->buffer = std::to_string(this->_fd.fd) + ": ";
	this->buffer = "";
}

void	User::appendBuffer(std::string msg)
{
	this->buffer += msg;
}

void	User::updateFullRef(void)
{
	this->_fullRef = this->_nickname + "!" + this->_username + "@" + this->_serv.getConfig().getServerName();
}



// Getters
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

const std::string	&User::getFullRef(void) const
{
	return (this->_nickname);
}

const bool			&User::getRegistered(void) const
{
	return (this->_registered);
}




// Setters
void				User::setUsername(const std::string &username)
{
	this->_username = username;
}

void				User::setNickname(const std::string &nickname)
{
	this->_nickname = nickname;
}

void				User::setRegistered(const bool &registered)
{
	this->_registered = registered;
}
