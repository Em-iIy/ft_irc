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
	this->_pfds.push_back(initPFD(STDIN_FILENO));
	this->_pfds.push_back(initPFD(this->_sock.getFd()));
	// this->_pfds[0].events = POLLIN;
	this->_sock.Init(this->_port);
	this->_sock.Bind();
	this->_sock.Listen(32);
	try
	{
		this->_log.init("ircserv");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
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
		this->_sock.Send(it->fd, ":" + this->getServerName() + " NOTICE all :Goodbye! o/ (Server turned off)\n");
		delete &this->_getUser(it->fd);
	}
}

void	Server::_checkPoll(void)
{
	for (pollfdIt it = this->_pfds.begin(); it != this->_pfds.end(); it++)
	{
		this->_pollOut(it);
		this->_pollIn(it);
	}
	// NOTE: moved connecting and disconnecting users outside the poll checking loop (could make new function alltogether)
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
	int			bRead = 1;
	char 		buffer[512];
	std::string	msg;

	if (!(it->revents & POLLIN))
		return ;
	// Server commands
	if (fd == STDIN_FILENO)
	{
		bRead = read(STDIN_FILENO, buffer, 512 - 1);
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
	bRead = recv(fd, buffer, 512 - 1, 0);
	// check whether recv returned 0 and disconnect user if it did
	if (this->_checkDc(bRead, it))
		return ;
	buffer[bRead] = '\0';
	std::cout << "User #" << fd << " recvd " << bRead << " bytes" << std::endl;
	User	&user = this->_getUser(fd);
	// Add read buffer to the back of the total message
	user.appendBuffer(buffer);
	//	loop through the buffer while it can find a \n (think getnextline)
	while (user.buffer.find("\n") != std::string::npos)
	{
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

void	Server::_pollOut(pollfdIt it)
{
	if (!(it->revents & POLLOUT))
		return ;
	if (it->fd == STDIN_FILENO)
		return ;
	// User is ready to recieve messages
	User	&user = this->_getUser(it->fd);
	// send all stored messages
	for (std::vector<std::string>::iterator msgIt = user.toSend.begin(); msgIt != user.toSend.end(); msgIt++)
		this->_sock.Send(user.getFd(), *msgIt);
	// clear all the messages
	user.toSend.clear();
}

bool	Server::_checkDc(int bRead, pollfdIt it)
{
	if (bRead > 0)
		return (false);
	// Set the user for disconnection
	User	&user = this->_getUser(it->fd);
	if (user.getRegistered())
		this->broadcastMsg(":" + user.getFullRef() + " QUIT :User disconnected\n");
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
