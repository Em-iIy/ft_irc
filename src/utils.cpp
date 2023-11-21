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

umode_e	cToUmode(char c)
{
	switch (c)
	{
	case 'a':
		return (UMODE_A);
	case 'i':
		return (UMODE_I);
	case 'w':
		return (UMODE_W);
	case 'r':
		return (UMODE_R);
	case 'o':
		return (UMODE_LO);
	case 'O':
		return (UMODE_UO);
	case 'S':
		return (UMODE_S);
	default:
		return (UMODE_NONE);
	}
}

std::string	umodeToStr(const umode_t mode)
{
	std::string	ret = "";

	if (mode == UMODE_NONE)
		return (ret);
	ret += "+";
	if (mode & UMODE_A)
		ret += "a";
	if (mode & UMODE_I)
		ret += "i";
	if (mode & UMODE_W)
		ret += "w";
	if (mode & UMODE_R)
		ret += "r";
	if (mode & UMODE_LO)
		ret += "o";
	if (mode & UMODE_UO)
		ret += "O";
	if (mode & UMODE_S)
		ret += "s";
	return (ret);
}