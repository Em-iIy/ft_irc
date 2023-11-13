#include "Server.hpp"

static pollfd initPFD(sockfd_t fd)
{
	pollfd ret;
	ret.fd = fd;
	ret.events = POLLIN | POLLOUT;
	return (ret);
}

Server::Server(const std::string &port, const std::string &password)try : _port(std::stoi(port)), _password(password)
{
	// Check for digits in port
	if (port.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("");
	// Check if port is within valid
	if (this->_port < 0 || this->_port > 65536)
		throw std::out_of_range("");
		this->_pfds.push_back(initPFD(this->_sock.getFd()));
	// this->_pfds[0].events = POLLIN;
	this->_sock.Init(this->_port);
	this->_sock.Bind();
	this->_sock.Listen(32);
	std::cout << "Server built on port: " << this->_port << std::endl;
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
	for (int i = 1; i < this->_pfds.size(); i++)
	{
		this->_sock.Send(this->_pfds[i].fd, "Goodbye! o/\n");
		close(this->_pfds[i].fd);
	}
}


void	Server::_acceptConn(void)
{
	pollfd	newSock = initPFD(this->_sock.Accept());

	if (newSock.fd < 0)
		throw std::runtime_error("Accept");
	User	*newUser = new User(*this, newSock, this->_pfds.size());
	// add fd to pfds
	this->_pfds.push_back(newSock);
	// add User fd pair to map
	this->_users.insert(std::pair<sockfd_t, User &>(newSock.fd, *newUser));
}

void	Server::disconnectUser(int i)
{
	// delete User class
	delete &this->_getUser(i);
	// remove User fd pair from map
	this->_users.erase(this->_pfds[i].fd);
	// remove fd from pfds
	this->_pfds.erase(this->_pfds.begin() + i);
}

bool	Server::_checkDc(int bRead, int i)
{
	if (bRead > 0)
		return (false);
	this->disconnectUser(i);
	return (true);
}

void	Server::_pollOut(int i)
{

	if (!(this->_pfds[i].revents & POLLOUT))
		return ;
	// User is ready to recieve messages
	User	&user = this->_getUser(i);
	// send all stored messages
	for (std::vector<std::string>::iterator it = user.toSend.begin(); it != user.toSend.end(); it++)
		this->_sock.Send(user.getFd(), *it);
	// clear all the messages
	user.toSend.clear();
}

void	Server::_pollIn(int i)
{
	int			fd = this->_pfds[i].fd;
	int			bRead = 1;
	char 		buffer[512];
	std::string	msg;

	if (!(this->_pfds[i].revents & POLLIN))
		return ;
	// check if it's the server socket
	if (fd == this->_sock.getFd())
	{
		this->_acceptConn();
		return ;
	}
	bRead = recv(fd, buffer, 512 - 1, 0);
	// check whether recv returned 0 and disconnect user if it did
	if (this->_checkDc(bRead, i))
		return ;
	buffer[bRead] = '\0';
	std::cout << "recvd " << bRead << " bytes" << std::endl;
	User	&user = this->_getUser(i);
	// Add read buffer to the back of the total message
	user.appendBuffer(buffer);
	// if CRLF is found, stop recv() and relay the message
	if (user.buffer.find("\r\n") != std::string::npos)
	{
		std::cout << "Full command: " << user.buffer;
		Message	msg(user.buffer, user, *this);
		// this->_relayMsg(user.buffer, i);
		// user.resetBuffer();
	}
}

void	Server::_checkPoll(void)
{
	int	npfd = this->_pfds.size();

	for (int i = 0; i < npfd; i++)
	{
		this->_pollOut(i);
		this->_pollIn(i);
	}
}

User	&Server::_getUser(int i)
{
	return (this->_users.at(this->_pfds[i].fd));
}

void	Server::Start(void)
{
	int rc;
	while (true)
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
	std::cout << "timeout" << std::endl;
}

bool	Server::checkPassword(const std::string &password) const
{
	return (this->_password == password);
}

// obsolete?
void	Server::relayMsg(std::string &msg, int i)
{
	int	npfd = this->_pfds.size();

	std::cout << msg;
	for (int j = 1; j < npfd; j++)
	{
		// add msg to users msg backlog
		this->_getUser(j).toSend.push_back(msg);
	}
}

// Returns true if the given nickname is already taken
bool		Server::checkNickname(const std::string nickname)
{
	std::vector<std::string>::iterator it = std::find(this->_nicknames.begin(), this->_nicknames.end(), nickname);

	return (it != this->_nicknames.end());
}

// Adds a new nickname to the vector
void		Server::addNickname(const std::string &nickname)
{
	this->_nicknames.push_back(nickname);
}

void		Server::removeNickname(const std::string &nickname)
{
	this->_nicknames.erase(std::find(this->_nicknames.begin(), this->_nicknames.end(), nickname));
}

// Getters
Config	&Server::getConfig(void)
{
	return (this->_config);
}

Socket	&Server::getSocket(void)
{
	return (this->_sock);
}