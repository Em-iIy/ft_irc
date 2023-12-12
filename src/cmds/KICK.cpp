#include "Message.hpp"

void	Message::_singleKICK(std::string &channelName, std::string &userNick, std::string &comment)
{
	Channel	*channel = this->_server.getChannel(channelName);
	User	*targetUser = this->_server.getUserFromNick(userNick);

	if (!channel)
	{
		// 403		ERR_NOSUCHCHANNEL
		this->_response = ":" + this->_server.getServerName() + " 403 " + this->_user.getNickname() + " " + channelName + " :No such channel\r\n";
		this->_respondUser();
		return ;
	}
	if (!channel->isUser(targetUser))
	{
		// 442		ERR_NOTONCHANNEL
		this->_response = ":" + this->_server.getServerName() + " 442 " + this->_user.getNickname() + " " + channelName + " :You're not on that channel\r\n";
		this->_respondUser();
		return ;
	}
	if (!channel->isOper(&this->_user))
	{
		// 482		ERR_CHANOPRIVSNEEDED
		this->_response = ":" + this->_server.getServerName() + " 482 " + this->_user.getNickname() + " " + channelName + " :You're not channel operator\r\n";
		this->_respondUser();
		return ;
	}
	if (!channel->isUser(targetUser))
	{
		// 441		ERR_USERNOTINCHANNEL
		this->_response = ":" + this->_server.getServerName() + " 441 " + this->_user.getNickname() + " " + userNick + " " + channelName + " :They aren't on that channel\r\n";
		this->_respondUser();
		return ;
	}
	channel->rmUser(targetUser);
	channel->rmWhitelist(targetUser);
	if (!comment.empty())
		comment = " :" + comment;
	this->_response = ":" + this->_user.getFullRef() + " KICK " + channelName + " " + userNick + comment + "\r\n";
	this->_respondChannel(channel);
	this->_response = ":" + this->_user.getFullRef() + " PART " + channelName + comment + "\r\n";
	this->_respondTargetUser(targetUser);
}

void	Message::_KICK(void)
{
	if (this->_params.size() < 2)
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_user.getNickname() + " " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}

	std::vector<std::string>	channels = split(this->_params[0], ",");
	std::vector<std::string>	users = split(this->_params[1], ",");
	std::string					comment;

	if (this->_params.size() >= 3)
		comment = this->_params[2];
	if (channels.size() > 1)
	{
		if (channels.size() != users.size())
		{
			// 461		ERR_NEEDMOREPARAMS
			this->_response = ":" + this->_server.getServerName() + " 461 " + this->_user.getNickname() + " " + this->_command + " :Not enough parameters\r\n";
			this->_respondUser();
			return ;
		}
		for (size_t i = 0; i < channels.size(); i++)
			this->_singleKICK(channels[i], users[i], comment);
	}
	else
		for (size_t i = 0; i < users.size(); i++)
			this->_singleKICK(channels[0], users[i], comment);
}