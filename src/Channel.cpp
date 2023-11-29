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

bool	Channel::_userInChannel(User *user)
{
	for (std::list<User *>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		if (*it == user)
			return true;
	}
	return false;
}

void	Channel::addUser(User *user)
{
	if (_userInChannel(user))
		return;
	this->_users.push_back(user);
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