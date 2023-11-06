#include "User.hpp"

User::User(pollfd fd): _fd(fd)
{

}

User::~User()
{
	std::cout << "bye bye " << this->_fd.fd << std::endl;
	close(this->_fd.fd);
}