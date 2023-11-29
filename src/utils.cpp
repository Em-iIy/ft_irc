#include "utils.hpp"
#include <ctime>

// Removes trailing \r\n from strings
void	rmCRLF(std::string &str)
{
	size_t CRLF = str.find_last_not_of("\r\n");

	if (CRLF == std::string::npos)
		return ;
	str.erase(CRLF + 1);
}

void	msgLimitSize(std::string &str)
{
	if (str.length() > MSG_BUFFERSIZE)
	{
		str.resize(MSG_BUFFERSIZE);
		str[MSG_BUFFERSIZE - 2] = '\r';
		str[MSG_BUFFERSIZE - 1] = '\n';
	}
}

// Initializes pollfd struct to the default settings
pollfd	initPFD(sockfd_t fd)
{
	pollfd ret;
	ret.fd = fd;
	ret.events = POLLIN | POLLOUT;
	return (ret);
}

// Return true if str has a channel prefix
bool	isChannel(const std::string &str)
{
	if (str[0] == '#' ||
		str[0] == '+' ||
		str[0] == '&' ||
		str[0] == '!')
		return (true);
	return (false);
}

// Converts a mode character to the corresponding enum value
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

// Converts a mode to a string with all the set modes
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

// Returns a date as a string formatted "Day Month DD YYYY"
std::string	date(void)
{
	std::time_t now = std::time(NULL);
	char	temp[32];

	std::strftime(temp, sizeof(temp), "%a %b %d %Y", std::localtime(&now));
	return (temp);
}

// Returns a time as a string formatted "HH:MM:SS"
std::string	time(void)
{
	std::time_t now = std::time(NULL);
	char	temp[9];

	std::strftime(temp, sizeof(temp), "%H:%M:%S", std::localtime(&now));
	return (temp);
}
