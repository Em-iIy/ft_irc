#include "Message.hpp"
#include <sstream>

// A-Z a-z 0-9 -_[]{}\`|
static bool	nickRules(char c)
{
	return (!(isalnum(c) || 
		(c >= '{' && c <= '}') || 
		(c >= '[' && c <= ']') || 
		c == '-' || 
		c == '_' || 
		c == '`'
		));
}

static bool	checkNickChars(std::string &nick)
{
	// Nickname must be 9 characters or smaller
	if (nick.length() > 9)
		return (false);
	// Nickname may not start with -
	if (nick[0] == '-')
		return (false);
	// Check nickname for valid characters
	return (std::find_if(nick.begin(), nick.end(), nickRules) == nick.end());
}


void	Message::_NICK(void)
{
	std::stringstream	param(this->_param);
	std::string			nick;
	
	std::getline(param, nick, ' ');
	if (nick == "")
	{
		// 431 ERR_NONICKNAMEGIVEN
		this->_response = ":" + this->_server.getConfig().getHostName() + " 431 :No nickname given\n";
		this->_respondUser();
		return ;
	}
	if (!checkNickChars(nick))
	{
		// 432 ERR_ERRONEUSNICKNAME
		this->_response = ":" + this->_server.getConfig().getHostName() + " 432 " + nick + " :Erroneus nickname\n";
		this->_respondUser();
		return ;
	}
	if (this->_server.checkNickname(nick) == true)
	{
		// 433 ERR_NICKNAMEINUSE
		this->_response = ":" + this->_server.getConfig().getHostName() + " 433 " + nick + " :Nickname is already in use\n";
		this->_respondUser();
		return ;
	}

	// std::cout << "getNickname(): " << _user.getNickname() << std::endl;
	if (this->_user.getNickname() == "")
	{
		this->_server.addNickname(nick);
		this->_user.setNickname(nick);
	}
	else
	{
		this->_response = ":" + this->_user.getNickname() + " NICK " + nick + "\n";
		this->_server.removeNickname(this->_user.getNickname());
		this->_server.addNickname(nick);
		this->_user.setNickname(nick);
		this->_server.relayMsg(this->_response, 0);
	}
}
