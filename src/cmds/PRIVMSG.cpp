#include "Message.hpp"

void	Message::_PRIVMSG(void)
{
	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
	if (this->_params.size() == 0)
	{
		// 411		ERR_NORECIPIENT
		this->_response = ":" + this->_server.getServerName() + " 411 " + this->_user.getNickname() + " :No recipient given (PRIVMSG)\r\n";
		this->_respondUser();
		return ;
	}
	if (isChannel(this->_params[0]) == false)
	{
		User *user = this->_server.getUserFromNick(this->_params[0]);
		if (!user)
		{
			// 401		ERR_NOSUCHNICK
			this->_response = ":" + this->_server.getServerName() + " 401 " + this->_user.getNickname() + " " + this->_params[0] + " :No such nick/channel\r\n";
			this->_respondUser();
			return ;
		}
		if (this->_params.size() == 1)
		{
			// 412		ERR_NOTEXTOTSEND
			this->_response = ":" + this->_server.getServerName() + " 412 " + this->_user.getNickname() + " :No text to send\r\n";
			this->_respondUser();
			return ;
		}
		if (user->checkMode(UMODE_A))
		{
			// 301		RPL_AWAY
			this->_response = ":" + this->_server.getServerName() + " 301 " + this->_user.getNickname() + " " + user->getNickname() + " :" + user->getAwayMsg() + "\r\n";
			this->_respondUser();
		}
		this->_response = ":" + this->_user.getFullRef() + " PRIVMSG " + this->_params[0] + " :" + this->_params[1] + "\r\n";
		this->_respondTargetUser(user);
	}
	else
	{
		Channel	*channel = this->_server.getChannel(this->_params[0]);
		if (!channel)
		{
			// 401		ERR_NOSUCHNICK
			this->_response = ":" + this->_server.getServerName() + " 401 " + this->_user.getNickname() + " " + this->_params[0] + " :No such nick/channel\r\n";
			this->_respondUser();
			return ;
		}
		if (this->_params.size() == 1)
		{
			// 412		ERR_NOTEXTOTSEND
			this->_response = ":" + this->_server.getServerName() + " 412 " + this->_user.getNickname() + " :No text to send\r\n";
			this->_respondUser();
			return ;
		}
		if (!channel->isUser(&this->_user))
		{
			// 404		ERR_CANNOTSENDTOCHAN
			this->_response = ":" + this->_server.getServerName() + " 404 " + this->_user.getNickname() + " " + this->_params[0] + " :Cannot send to channel\r\n";
			this->_respondUser();
			return ;
		}
		this->_response = ":" + this->_user.getFullRef() + " PRIVMSG " + this->_params[0] + " :" + this->_params[1] + "\r\n";
		for (std::list<User *>::iterator it = channel->getUsers().begin(); it != channel->getUsers().end(); ++it)
		{
			if (*it != &this->_user)
				this->_respondTargetUser(*it);
		}
	}
}