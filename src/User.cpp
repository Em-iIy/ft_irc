#include "User.hpp"

std::ostream	&operator<<(std::ostream &o, const User &rhs)
{
	o << "User #" << rhs.getFd() << " - " << rhs.getNickname() <<  ":" << std::endl;
	o << "Fd:\t\t" << rhs.getFd() << std::endl;
	o << "Nickname:\t" << rhs.getNickname() << std::endl;
	o << "Username:\t" << rhs.getUsername() << std::endl;
	o << "Hostname:\t" << rhs.getHostname() << std::endl;
	o << "Servername:\t" << rhs.getServername() << std::endl;
	o << "Realname:\t" << rhs.getRealname() << std::endl;
	o << "FullRef:\t" << rhs.getFullRef() << std::endl;
	return (o);
}

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

void	User::registerUser(std::string &username, std::string &hostname, std::string &servername, std::string &realname)
{
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
	this->toSend.push_back(":" + this->_serv.getHostName() + " 001 " + this->_nickname + " :Welcome to the Internet Relay Network " + this->_fullRef + "\n");
}

void	User::updateFullRef(void)
{
	this->_fullRef = this->_nickname + "!" + this->_username + "@" + this->_serv.getServerName();
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

const bool			User::getRegistered(void) const
{
	return (this->_status & STAT_REG);
}

const bool			User::getPassword(void) const
{
	return (this->_status & STAT_REG_PASS);

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

void				User::capStart(void)
{
	this->_status |= STAT_CAP_START;
}

void				User::capEnd(void)
{
	this->_status |= STAT_CAP_END;
}

