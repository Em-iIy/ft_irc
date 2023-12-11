#include "Channel.hpp"
#include "User.hpp"

Channel::Channel(std::string &name, std::string &pass, User *creator, Server &server) : _server(server), _name(name)
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
	this->_server.rmChannel(this);
}

// Getters
const std::string	&Channel::getName(void)
{
	return (this->_name);
}

const std::string	&Channel::getPass(void)
{
	return (this->_pass);
}

const std::string	&Channel::getTopic(void)
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

cmode_t				&Channel::getMode(void)
{
	return (this->_mode);
}

int				Channel::getLimit(void)
{
	return (this->_mode & 0xffff);
}

bool				Channel::checkMode(cmode_t mode) const
{
	return (this->_mode & mode);
}


// Setters
void				Channel::setTopic(const std::string &topic)
{
	this->_topic = topic;
}


bool	Channel::isUser(User *user)
{
	if (!user)
		return false;
	for (std::list<User *>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		if (*it == user)
			return true;
	}
	return false;
}

bool	Channel::isOper(User *user)
{
	if (!user)
		return false;
	for (std::list<User *>::iterator it = this->_opers.begin(); it != this->_opers.end(); ++it)
	{
		if (*it == user)
			return true;
	}
	return false;
}

void	Channel::addUser(User *user, std::string &pass)
{
	if (this->isUser(user))
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

bool	Channel::rmUser(User *user)
{
	this->_users.remove(user);
	if (this->isOper(user))
		this->rmOper(user);
	// also removes channel from users' own list
	user->rmFromChannel(this);
	if (!this->getUsers().size())
	{
		this->_server.rmChannel(this);
		delete (this);
		return (true);
	}
	return (false);
}

void				Channel::addOper(User *user)
{
	if (this->isOper(user))
		return;
	this->_opers.push_back(user);
}

void				Channel::rmOper(User *user)
{
	this->_opers.remove(user);
	if (!(this->_opers.size()) && (this->_users.size() > 0))
		this->addOper(*(this->_users.begin()));
}

void				Channel::addMode(cmode_t mode)
{
	this->_mode |= mode;
}

void				Channel::rmMode(cmode_t mode)
{
	this->_mode &= ~mode;
}

void				Channel::setPass(std::string pass)
{
	this->_pass = pass;
}

void				Channel::setLimit(int limit)
{
	if (limit > 0xffff)
		limit = 0xffff;
	this->_mode &= ~0xffff;
	this->_mode |= limit;
}

void				Channel::setMode(cmode_t &mode)
{
	this->_mode = mode;
}

void				Channel::setOpers(std::list<User *> &opers)
{
	this->_opers = opers;
}

// Returns pointer to user with certain nickname
User				*Channel::getUserFromNick(std::string &nick)
{
	for (std::list<User *>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		if ((*it)->getNickname() == nick)
			return (*it);
	}
	return (NULL);
}

std::ostream	&operator<<(std::ostream &out, Channel &c)
{
	out << "Channel " << c.getName() << ":" << std::endl <<
	"Topic: " << c.getTopic() << std::endl <<
	"Users: " << c.getUsers().size() << std::endl;
	for (std::list<User *>::iterator it = c.getUsers().begin(); it != c.getUsers().end(); ++it)
		out << (**it).getNickname() << std::endl;
	out << "Operators:" << c.getOpers().size() << std::endl;
	for (std::list<User *>::iterator it = c.getOpers().begin(); it != c.getOpers().end(); ++it)
		out << (**it).getNickname() << std::endl;
	out << "User limit: " << c.getLimit() << std::endl;
	out << "Password: " << c.getPass() << std::endl;
	return out;
}