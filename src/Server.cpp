#include "Server.hpp"
#include "serverCmds.hpp"
#include "utils.hpp"
#include <sstream>

Server::Server(const std::string &port, const std::string &password)try : _port(std::stoi(port)), _password(password)
{
	// Check for digits in port
	if (port.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("");
	// Check if port is within valid
	if (this->_port < 0 || this->_port > 65536)
		throw std::out_of_range("");
	// Check if password isn't an empty string, making it impossible to register
	if (this->_password == "")
		throw std::runtime_error("invalid password: can't be an empty string");
	// Add STDIN to write commands in from the terminal
	this->_pfds.push_back(initPFD(STDIN_FILENO));
	this->_pfds.push_back(initPFD(this->_sock.getFd()));
	this->_sock.Init(this->_port);
	this->_sock.Bind();
	this->_sock.Listen(32);
	// Try to initialize log file (does not neet to exist for server to run)
	try
	{
		this->_log.init("ircserv");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	this->_pingTimer = std::time(NULL);
	this->log("Server", "Server built on port: " + port);
	
}
catch(const std::invalid_argument& e)
{
	throw std::runtime_error("invalid port: must be a number");
}
catch(const std::out_of_range& e)
{
	throw std::runtime_error("invalid port: out of range [0:65536]");
}

Server::~Server()
{
	// Loop through all user pfds
	for (pollfdIt it = this->_pfds.begin() + 2; it != this->_pfds.end(); it++)
	{
		#ifdef DEBUG_MODE
			// Avoids using send() without calling poll() when not debugging
			try
			{
				this->_sock.Send(it->fd, ":" + this->getServerName() + " NOTICE all :Goodbye! o/ (Server turned off)\r\n");
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
		#endif
		delete &this->_getUser(it->fd);
	}
	// Loop through all channels
	for (std::list<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
		delete *it;
}

void	Server::_checkPoll(void)
{
	for (pollfdIt it = this->_pfds.begin(); it != this->_pfds.end(); it++)
	{
		if (this->_pollOut(it) == true)
			continue ;
		this->_pollIn(it);
	}
	// If any, add pollfds of new connections to the full list of connections
	if (this->_newPfds.size())
		this->_connectPfds();
	// If any, disconnect users associated with the pollfd in the dcPfds list
	if (this->_dcPfds.size())
		this->_disconnectPfds();
}

void	Server::_pollIn(pollfdIt it)
{
	int			fd = it->fd;
	ssize_t		bRead = 1;
	char 		buffer[MSG_BUFFERSIZE];
	std::string	msg;

	if (!(it->revents & POLLIN))
		return ;
	// Server commands
	if (fd == STDIN_FILENO)
	{
		bRead = read(STDIN_FILENO, buffer, MSG_BUFFERSIZE - 1);
		buffer[bRead] = '\0';
		serverCmd(buffer, *this);
		return ;
	}
	// check if it's the server socket
	if (fd == this->_sock.getFd())
	{
		this->_acceptConn();
		return ;
	}
	bRead = recv(fd, buffer, MSG_BUFFERSIZE - 1, 0);
	// check whether recv returned 0 and disconnect user if it did
	if (this->_checkDc(bRead, it))
		return ;
	buffer[bRead] = '\0';
	std::cout << "User #" << fd << " recvd " << bRead << " bytes" << std::endl;
	User	&user = this->_getUser(fd);
	// Add read buffer to the back of the total message
	user.appendBuffer(buffer);
	// Check if the received message contains \n from the back (if it doesn't then the user.buffer won't have one either, no need to check that again)
	if (std::find(std::rbegin(buffer) + (MSG_BUFFERSIZE - bRead), std::rend(buffer), '\n') == std::rend(buffer))
		return ;
	//	loop through the buffer while it can find a \n (think getnextline)
	while (user.buffer.find_last_of("\n") != std::string::npos)
	{
		msgLimitSize(user.buffer);
		std::stringstream	temp(user.buffer);
		std::string command;

		// Separate out the first line
		std::getline(temp, command, '\n');
		// Store the rest
		std::getline(temp, user.buffer, '\0');
		// Process command
		Message	msg(command, user, it, *this);
	}
}

int	Server::_pollOut(pollfdIt it)
{
	if (!(it->revents & POLLOUT))
		return (false);
	if (it->fd == STDIN_FILENO)
		return (false);
	// User is ready to recieve messages
	User	&user = this->_getUser(it->fd);
	// Send top message
	if (user.toSend.size() == 0)
		return (false);
	try
	{
		this->_sock.Send(user.getFd(), *user.toSend.begin());
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	// clear top message
	user.toSend.erase(user.toSend.begin());
	return (true);
}

bool	Server::_checkDc(ssize_t bRead, pollfdIt it)
{
	if (bRead > 0)
		return (false);
	User	&user = this->_getUser(it->fd);
	if (user.getRegistered())
	{
		std::list<Channel *>	&channels = user.getChannels();
		std::string				msg = " :User disconnected\r\n";
		std::string				prefix = ":" + user.getFullRef() + " QUIT ";
		std::string				response;

		for (std::list<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			response = prefix + (*it)->getName() + msg;
			for (std::list<User *>::iterator itChannel = (*it)->getUsers().begin(); itChannel != (*it)->getUsers().end(); ++itChannel)
				(*itChannel)->toSend.push_back(response);
		}
	}
	// Set the user for disconnection
	this->_dcPfds.push_back(*it);
	return (true);
}

void	Server::_disconnectPfds(void)
{
	// Loop through all _dcPfds
	for (pollfdIt dcIt = this->_dcPfds.begin(); dcIt != this->_dcPfds.end(); dcIt++)
	{
		// Find the iterator for the corresponding pdf and disconnect user
		for (std::vector<pollfd>::iterator it = this->_pfds.begin(); it != this->_pfds.end(); it++)
		{
			if (it->fd == dcIt->fd)
			{
				this->disconnectUser(it);
				break ;
			}
		}
	}
	this->_dcPfds.clear();
}

void	Server::_acceptConn(void)
{
	std::pair<sockfd_t, sockaddr_in>	newConn;
	pollfd 								newSock;

	try
	{
		newConn = this->_sock.Accept();
		newSock = initPFD(newConn.first);
	}
	catch(...)
	{
		std::cout << "Couldn't connect user" << std::endl;
		return ;
	}
	User	*newUser = new User(*this, newSock, newConn.second);
	// add fd to newPfds
	this->_newPfds.push_back(newSock);
	// add User fd pair to map
	this->_users.insert(std::pair<sockfd_t, User &>(newSock.fd, *newUser));
}

void	Server::_connectPfds(void)
{
	// Append the _newPfds vector to the _pfds vector
	this->_pfds.insert(this->_pfds.end(), this->_newPfds.begin(), this->_newPfds.end());
	this->_newPfds.clear();
}

void	Server::_pingUsers(void)
{
	// Check whether it's time to PING users
	if (std::time(NULL) - this->_pingTimer < PING_FREQ)
		return ;
	std::string msg = "PING " + this->getServerName() + "\r\n";
	// Loop through all users (except for the server socket and STDIN)
	for (pollfdIt it = this->_pfds.begin() + 2; it != this->_pfds.end(); it++)
	{
		User	&user = this->_getUser(it->fd);
		// Disconnect user if they've been idle for too long
		if (user.getIdle() > IDLE_TIMEOUT)
			this->addDcPfd(it);
		user.toSend.push_back(msg);
	}
	this->_pingTimer = std::time(NULL);
}

User	&Server::_getUser(int fd)
{
	return (this->_users.at(fd));
}

void	Server::Start(void)
{
	int rc;
	this->_running = true;
	while (this->_running)
	{
		this->_pingUsers();
		// poll all the pfds for any events
		rc = poll(this->_pfds.data(), this->_pfds.size(), 100000);
		if (rc < 0)
			throw std::runtime_error("poll");
		else if (rc == 0)
			break ;
		// check what events happened
		this->_checkPoll();
	}
	std::cout << ":stopping server:" << std::endl;
}

void	Server::Stop(void)
{
	this->_running = false;
}

void	Server::log(const std::string &src, const std::string &str)
{
	this->_log.logToFile(src, str);
}

bool	Server::checkPassword(const std::string &password) const
{
	return (this->_password == password);
}

void	Server::disconnectUser(pollfdIt it)
{
	User	&user = this->_getUser(it->fd);
	// remove nickname from _nicknames
	this->removeNickname(user.getNickname());
	// delete User class
	delete &user;
	// remove User fd pair from map
	this->_users.erase(it->fd);
	// remove fd from pfds
	this->_pfds.erase(it);
}

void	Server::addDcPfd(pollfdIt &it)
{
	this->_dcPfds.push_back(*it);
}

void	Server::broadcastMsg(const std::string &msg)
{
	this->log("Server", msg);
	for (pollfdIt it = this->_pfds.begin() + 2; it != this->_pfds.end(); it++)
	{
		// add msg to users msg backlog
		this->_getUser(it->fd).toSend.push_back(msg);
	}
}

// Returns true if the given nickname is already taken
bool	Server::checkNickname(const std::string &nickname)
{
	if (this->_nicknames.size() == 0)
		return (false);
	std::vector<std::string>::iterator it = std::find(this->_nicknames.begin(), this->_nicknames.end(), nickname);

	return (it != this->_nicknames.end());
}

// Adds a new nickname to the vector
void	Server::addNickname(const std::string &nickname)
{
	this->_nicknames.push_back(nickname);
}

void	Server::removeNickname(const std::string &nickname)
{
	if (this->_nicknames.size() == 0)
		return ;
	this->_nicknames.erase(std::find(this->_nicknames.begin(), this->_nicknames.end(), nickname));
}

// Channel stuff

// Returns the new channel pointer
Channel	*Server::addChannel(std::string &name, std::string &pass, User *creator)
{
	Channel *newChannel = new Channel(name, pass, creator, *this);

	// Do these 2 lines need to be there?
	newChannel->addUser(creator, pass);
	newChannel->addOper(creator);

	// Add newChannel to the list of all channels
	this->_channels.push_back(newChannel);
	return (newChannel);
}

void	Server::rmChannel(Channel *channel)
{
	this->_channels.remove(channel);
}

bool	Server::hasChannel(std::string &name)
{
	for (std::list<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
	{
		if ((**it).getName() == name)
			return (true);
	}
	return (false);
}

Channel	*Server::getChannel(std::string &name)
{
	for (std::list<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
	{
		if ((**it).getName() == name)
			return (*it);
	}
	return (NULL);
}

void	Server::printChannels(void)
{
	for (std::list<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
		std::cout << **it << std::endl;
}

// Getters
const Config	&Server::getConfig(void) const
{
	return (this->_config);
}

const std::string	&Server::getHostName(void) const
{
	return (this->_config.getHostName());
}

const std::string	&Server::getStartDate(void) const
{
	return (this->_config.getDate());
}

const std::string	&Server::getServerName(void) const
{
	return (this->_config.getServerName());
}

const std::string	&Server::getVersion(void) const
{
	return (this->_config.getVersion());
}

const std::string	&Server::getDebug(void) const
{
	return (this->_config.getDebug());
}

const std::string	&Server::getComments(void) const
{
	return (this->_config.getComments());
}

Socket	&Server::getSocket(void)
{
	return (this->_sock);
}

std::vector<std::string> 	&Server::getNicknames(void)
{
	return (this->_nicknames);
}

std::map<sockfd_t, User &>	&Server::getUsers(void)
{
	return (this->_users);
}

std::list<Channel *>	&Server::getChannels(void)
{
	return (this->_channels);
}

// Returns pointer to user with certain nickname
User	*Server::getUserFromNick(std::string &nick)
{
	for (std::map<sockfd_t, User &>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		if ((*it).second.getNickname() == nick)
			return (&(*it).second);
	}
	return (NULL);
}