#include "utils.hpp"
#include "types.hpp"
#include <ctime>
#include <list>

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

// Converts a user mode character to the corresponding enum value
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
	case 's':
		return (UMODE_S);
	default:
		return (UMODE_NONE);
	}
}

// Converts a channel mode character to the corresponding enum value
cmode_e	cToCmode(char c)
{
	switch (c)
	{
	case 'i':
		return (CMODE_I);
	case 't':
		return (CMODE_T);
	case 'k':
		return (CMODE_K);
	case 'o':
		return (CMODE_O);
	case 'l':
		return (CMODE_L);
	default:
		return (CMODE_NONE);
	}
}

// Converts a user mode to a string with all the set modes
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

// Converts a channel mode to a string with all the set modes
std::string	cmodeToStr(const cmode_t mode)
{
	std::string	ret = "";

	if (mode == CMODE_NONE)
		return (ret);
	ret += "+";
	if (mode & CMODE_I)
		ret += "i";
	if (mode & CMODE_T)
		ret += "t";
	if (mode & CMODE_K)
		ret += "k";
	if (mode & CMODE_O)
		ret += "o";
	if (mode & CMODE_L)
		ret += "l";
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

// Returns a vector of strings from a string seperated by commas
std::vector<std::string>	parseParamByComma(std::string param)
{
	size_t						comma = 0;
	std::vector<std::string>	paramsVector;

	if (param.empty())
		return (paramsVector);
	if (param.find(',') == std::string::npos)
		paramsVector.push_back(param);
	else
	{
		while ((comma = param.find(',')) != std::string::npos)
		{
			paramsVector.push_back(param.substr(0, comma));
			param.erase(0, comma + 1);
		}
	}
	return (paramsVector);
}

std::vector<std::string>	split(const std::string &str, const std::string &d)
{
	size_t	pos = 0;
	size_t	find = str.find(d);
	std::vector<std::string> ret;

	if (find == std::string::npos)
	{
		ret.push_back(str);
		return (ret);
	}
	while (find != std::string::npos)
	{
		ret.push_back(str.substr(pos, find - pos));
		pos = find + d.length();
		find = str.find(d, pos);
	}
	ret.push_back(str.substr(pos));
	return (ret);
}

bool	inTempOpers(std::list<User *> opers, User *user)
{
	for (std::list<User *>::iterator it = opers.begin(); it != opers.end(); ++it)
	{
		if (*it == user)
			return (true);
	}
	return (false);
}