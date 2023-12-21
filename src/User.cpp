#include "User.hpp"

std::ostream	&operator<<(std::ostream &o, User &rhs)
{
	o << "User #" << rhs.getFd() << " - " << rhs.getNickname() <<  ":" << std::endl;
	o << "Fd:\t\t" << rhs.getFd() << std::endl;
	o << "IP:\t\t" << rhs.getIP() << std::endl;
	o << "Port:\t\t" << rhs.getPort() << std::endl;
	o << "Nickname:\t" << rhs.getNickname() << std::endl;
	o << "Username:\t" << rhs.getUsername() << std::endl;
	o << "Hostname:\t" << rhs.getHostname() << std::endl;
	o << "Servername:\t" << rhs.getServername() << std::endl;
	o << "Realname:\t" << rhs.getRealname() << std::endl;
	o << "FullRef:\t" << rhs.getFullRef() << std::endl;
	o << "Mode:\t\t" << umodeToStr(rhs.getMode()) << std::endl;
	o << "Idle:\t\t" << rhs.getIdle() << " seconds" << std::endl;
	o << "Channels:\t";
	for (std::list<Channel *>::iterator it = rhs.getChannels().begin(); it != rhs.getChannels().end(); ++it)
		o << (*it)->getName() << " ";
	o << std::endl;
	return (o);
}

User::User(Server &serv, pollfd fd, sockaddr_in sock): _serv(serv), _fd(fd), _sock(sock)
{
	std::cout << "User #" << this->_fd.fd << " Connected." << std::endl;
	this->resetBuffer();
	this->_username = "";
	this->_nickname = "";
	this->_awayMsg = "";
	this->_idleTime = std::time(NULL);
}

User::~User()
{
	std::cout << "User #" << this->_fd.fd << " Disconnected." << std::endl;
	while (this->_channels.size())
	{
		// Remove the user from all the channels they were a part of
		(*this->_channels.begin())->rmWhitelist(this);
		(*this->_channels.begin())->rmUser(this);
	}
	close(this->_fd.fd);
}

void	User::resetBuffer(void)
{
	this->buffer = "";
}

void	User::appendBuffer(std::string msg)
{
	this->buffer += msg;
}

void	User::registerUser(std::string &username, std::string &hostname, std::string &servername, std::string &realname)
{
	// Register all user info, update status and fullref
	this->_username = username;
	this->_hostname = hostname;
	this->_servername = servername;
	this->_realname = realname;
	this->_status |= STAT_REG_USER;
	this->updateFullRef();
}

// Updates registered and sends welcome message
void	User::checkRegister(void)
{
	if (this->getRegistered())
		return ;
	// Check if CAP negotiations was started it was also ended
	if (this->_status & STAT_CAP_START && !(this->_status & STAT_CAP_END))
		return ;
	// Check if a nickname was registered
	if (!(this->_status & STAT_REG_NICK))
		return ;
	// Check if user info was registered
	if (!(this->_status & STAT_REG_USER))
		return ;
	this->_status |= STAT_REG;
	// Send server welcome messages to the user
	this->toSend.push_back(":" + this->_serv.getServerName() + " 001 " + this->_nickname + " :Welcome to the Codam Chat Network " + this->_fullRef + "\r\n");
	this->toSend.push_back(":" + this->_serv.getServerName() + " 002 " + this->_nickname + " :Your host is " + this->_serv.getServerName() + ", running version " + this->_serv.getVersion() + "\r\n");
	this->toSend.push_back(":" + this->_serv.getServerName() + " 003 " + this->_nickname + " :This server was created " + this->_serv.getStartDate() + "\r\n");
	this->toSend.push_back(":" + this->_serv.getServerName() + " 004 " + this->_nickname + " "  + this->_serv.getServerName() + " " + this->_serv.getVersion() + " aoOirw itkol" + "\r\n"); // <user modes> <channel modes>
}

void	User::updateFullRef(void)
{
	this->_fullRef = this->_nickname + "!" + this->_username + "@" + this->_serv.getHostName();
}



// Getters
const int &User::getFd(void) const
{
	return (this->_fd.fd);
}

std::string	User::getIP(void) const
{
	return (inet_ntoa(this->_sock.sin_addr));
}

int	User::getPort(void) const
{
	return (this->_sock.sin_port);
}


const std::string	&User::getUsername(void) const
{
	return (this->_username);
}

const std::string	&User::getNickname(void) const
{
	return (this->_nickname);
}

const std::string	&User::getHostname(void) const
{
	return (this->_hostname);
}

const std::string	&User::getServername(void) const
{
	return (this->_servername);
}

const std::string	&User::getRealname(void) const
{
	return (this->_realname);
}

const std::string	&User::getFullRef(void) const
{
	return (this->_fullRef);
}

const std::string	&User::getAwayMsg(void) const
{
	return (this->_awayMsg);
}

bool			User::getRegistered(void) const
{
	return (this->_status & STAT_REG);
}

bool			User::getPassword(void) const
{
	return (this->_status & STAT_REG_PASS);

}

const umode_t	&User::getMode(void) const
{
	return (this->_mode);
}

bool			User::checkMode(umode_t mode) const
{
	return (this->_mode & mode);
}

std::time_t		User::getIdle(void) const
{
	return (std::time(NULL) - this->_idleTime);
}

std::list<Channel *>	&User::getChannels(void)
{
	return (this->_channels);
};

Channel 				*User::getChannel(const std::string &channelName)
{
	for (std::list<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
		if ((*it)->getName() == channelName)
			return (*it);
	return (NULL);
}



// Setters
void				User::setUsername(const std::string &username)
{
	this->_username = username;
}

void				User::setNickname(const std::string &nickname)
{
	this->_nickname = nickname;
	this->_status |= STAT_REG_NICK;
	this->updateFullRef();
}

void				User::setAwayMsg(const std::string &msg)
{
	this->_awayMsg = msg;
}

void				User::setRegistered(const bool &registered)
{
	if (registered)
		this->_status |= STAT_REG;
}

void				User::setPassword(const bool &password)
{
	if (password)
		this->_status |= STAT_REG_PASS;
}

void				User::addMode(umode_t mode)
{
	this->_mode |= mode;
}

void				User::rmMode(umode_t mode)
{
	this->_mode &= ~mode;
}

void				User::setIdle(const std::time_t &time)
{
	this->_idleTime = time;
}


void				User::capStart(void)
{
	this->_status |= STAT_CAP_START;
}

void				User::capEnd(void)
{
	this->_status |= STAT_CAP_END;
}

void				User::addToChannel(Channel *channel)
{
	this->_channels.push_back(channel);
}

void				User::rmFromChannel(Channel *channel)
{
	this->_channels.remove(channel);
}