#include "Channel.hpp"

Channel::Channel(std::string &name, std::string &pass, User *creator) : _name(name)
{
	if (!pass.empty())
	{
		this->addMode(CMODE_K);
		this->_pass = pass;
	}
	this->_users.push_back(creator);
	this->_opers.push_back(creator);
}

Channel::~Channel(void)
{
}

const std::string	&Channel::getName(void)
{
	return (this->_name);
}

const std::string	&Channel::getPass(void)
{
	return (this->_pass);
}

void	Channel::addMode(cmode_t mode)
{
	this->_mode |= mode;
}

void	Channel::rmMode(cmode_t mode)
{
	this->_mode &= ~mode;
}