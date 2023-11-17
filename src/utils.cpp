#include "utils.hpp"

// removes trailing \r\n from strings
void	rmCRLF(std::string &str)
{
	size_t CRLF = str.find_last_not_of("\r\n");

	if (CRLF == std::string::npos)
		return ;
	str.erase(CRLF + 1);
}

pollfd	initPFD(sockfd_t fd)
{
	pollfd ret;
	ret.fd = fd;
	ret.events = POLLIN | POLLOUT;
	return (ret);
}