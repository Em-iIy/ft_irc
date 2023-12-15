#include "Message.hpp"

void	Message::_PART(void)
{
	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
	if (!this->_params.size())
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_user.getNickname() + " " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}

	std::vector<std::string>	channelsToPart = split(this->_params[0], ",");

	for (std::vector<std::string>::iterator it = channelsToPart.begin(); it != channelsToPart.end(); ++it)
	{
		Channel	*channel = this->_server.getChannel(*it);
		if (!channel)
		{
			// 403		ERR_NOSUCHCHANNEL
			this->_response = ":" + this->_server.getServerName() + " 403 " + this->_user.getNickname() + " " + *it + " :No such channel\r\n";
			this->_respondUser();
			continue ;
		}
		if (!channel->isUser(&(this->_user)))
		{
			// 442		ERR_NOTONCHANNEL
			this->_response = ":" + this->_server.getServerName() + " 442 " + this->_user.getNickname() + " " + channel->getName() + " :You're not on that channel\r\n";
			this->_respondUser();
			continue ;
		}
		// Announce to the channel the user is leaving
		this->_response = ":" + this->_user.getFullRef() + " PART " + channel->getName();
		if (this->_params.size() > 1)
			this->_response += " :" + this->_params[1];
		this->_response += "\r\n";
		this->_respondChannel(channel);
		if (channel->rmUser(&(this->_user)))
			continue ;
	}
}