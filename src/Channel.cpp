#include "Channel.hpp"
#include "User.hpp"

Channel::Channel(std::string &name, std::string &pass, User *creator) : _name(name)
{
	if (!pass.empty())
	{
		this->addMode(CMODE_K);
		this->_pass = pass;
	}
	this->_users.push_back(creator);
	this->_opers.push_back(creator);
	creator->addToChannel(this);
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

void	Channel::addUser(User *user, std::string &pass)
{
	if (_userInChannel(user))
		return;
	if (this->_mode & CMODE_K)
		if (pass == this->_pass)
		{
			this->_users.push_back(user);
			user->addToChannel(this);
		}
		else
			throw std::exception();
	else
	{
		this->_users.push_back(user);
		user->addToChannel(this);
	}
}

const std::string	&Channel::getName(void)
{
	return (this->_name);
}

const std::string	&Channel::getPass(void)
{
	return (this->_pass);
}

const std::string		&Channel::getTopic(void)
{
	return (this->_topic);
}

 std::list<User *>	&Channel::getUsers(void)
{
	return (this->_users);
}

std::list<User *>	&Channel::getOpers(void)
{
	return (this->_opers);
}

void	Channel::addMode(cmode_t mode)
{
	this->_mode |= mode;
}

void	Channel::rmMode(cmode_t mode)
{
	this->_mode &= ~mode;
}

std::ostream	&operator<<(std::ostream &out, Channel &c)
{
	out << "Channel " << c.getName() << std::endl <<
	"Topic: " << c.getTopic() << std::endl <<
	"Users: " << c.getUsers().size() << std::endl;
	for (std::list<User *>::iterator it = c.getUsers().begin(); it != c.getUsers().end(); ++it)
		out << (**it).getUsername() << std::endl;
	out << "Operators:" << c.getOpers().size() << std::endl;
	for (std::list<User *>::iterator it = c.getOpers().begin(); it != c.getOpers().end(); ++it)
		out << (**it).getUsername() << std::endl;
	return out;
}