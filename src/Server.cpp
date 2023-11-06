#include "Server.hpp"

static pollfd initPFD(sockfd_t fd)
{
	pollfd ret;
	ret.fd = fd;
	ret.events = POLLIN;
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

void	Server::_relayMsg(std::string &msg, int i)
{
	int	npfd = this->_pfds.size();

	std::cout << msg;
	for (int j = 1; j < npfd; j++)
	{
		this->_sock.Send(this->_pfds[j].fd, msg);
	}
}

bool	Server::_checkDc(int i)
{
	if (this->_pfds[i].revents & POLLHUP)
	{
		std::cout << this->_pfds[i].fd << " hung up" << std::endl;
		delete &this->_getUser(i);
		this->_pfds.erase(this->_pfds.begin() + i);
		return (true);
	}
	return (false);
}

void	Server::_acceptConn(void)
{
	pollfd	newSock = initPFD(this->_sock.Accept());
	User	*newUser = new User(newSock);

	if (newSock.fd < 0)
		throw std::runtime_error("Accept");
	this->_pfds.push_back(newSock);
	this->_users.insert(std::pair<sockfd_t, User &>(newSock.fd, *newUser));
	std::cout << "Socket fd " << newSock.fd << " accepted" << std::endl;
	this->_sock.Send(newSock.fd, "Welcome!\n");
}

void	Server::_pollIn(int i)
{
	int			fd = this->_pfds[i].fd;
	int			bRead = 1;
	char 		buffer[1024];
	std::string	msg;

	if (fd == this->_sock.getFd())
	{
		this->_acceptConn();
		return ;
	}
	User	&user = this->_getUser(i);
	msg = std::to_string(fd) + ": ";
	bRead = recv(fd, buffer, 1024 - 1, 0);
	buffer[bRead] = '\0';
	user.buffer += buffer;
	// if newline is found, stop recv() and relay the message
	if (user.buffer.find('\n') != std::string::npos)
	{
		// std::cout << user.buffer << std::endl;
		this->_relayMsg(user.buffer, i);
	}
}

void	Server::_checkPoll(void)
{
	int	npfd = this->_pfds.size();

	for (int i = 0; i < npfd; i++)
	{
		this->_checkDc(i);
		switch (this->_pfds[i].revents)
		{
		case POLLIN:
			this->_pollIn(i);
			break ;
		case POLLOUT:
			throw std::runtime_error("poll: invalid event");
		case 0:
			break ;
		default:
			break ;
		}
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
		rc = poll(this->_pfds.data(), this->_pfds.size(), 100000);
		if (rc < 0)
			throw std::runtime_error("poll");
		else if (rc == 0)
			break ;
		this->_checkPoll();
	}
	std::cout << "timeout" << std::endl;
}