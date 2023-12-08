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
		channel->rmUser(&(this->_user));
		// If operator leaves, remove them as such from the channel as well
		if (channel->isOper(&(this->_user)))
			channel->rmOper(&(this->_user));
		// Announce to the channel the user is leaving
		this->_response = ":" + this->_user.getFullRef() + " PART " + channel->getName();
		if (this->_params.size() > 1)
			this->_response += " :" + this->_params[1];
		this->_response += "\r\n";
		this->_respondChannel(channel);
		if (!channel->getUsers().size())
		{
			delete (channel);
			this->_server.rmChannel(channel);
		}
	}
}

// void	Message::_PART(void)
// {
// 	// Must be registered to use this command
// 	if (this->_user.getRegistered() == false)
// 		return ;
// 	if (!this->_params.size())
// 	{
// 		// 461		ERR_NEEDMOREPARAMS
// 		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_command + " :Not enough parameters\r\n";
// 		this->_respondUser();
// 		return ;
// 	}

// 	std::vector<std::string>	channelsToPart = parseParamByComma(this->_params[0]);
// 	std::list<Channel *>		allChannels = this->_server.getChannels();

// 	for (size_t i = 0; i < channelsToPart.size(); i++)
// 	{
// 		for (std::list<Channel *>::iterator it = allChannels.begin(); it != allChannels.end(); ++it)
// 		{
// 			if (channelsToPart[i] == (**it).getName())
// 			{
// 				if (!(**it).isUser(&(this->_user)))
// 				{
// 					// 442		ERR_NOTONCHANNEL
// 					this->_response = ":" + this->_server.getServerName() + " 442 " + (**it).getName() + " :You're not on that channel\r\n";
// 					this->_respondUser();
// 					break ;
// 				}
// 				// If operator leaves, remove them as such from the channel as well
// 				if ((**it).isOper(&(this->_user)))
// 					(**it).rmOper(&(this->_user));
// 				(**it).rmUser(&(this->_user));
// 				// Announce to the channel the user is leaving
// 				this->_response = ":" + this->_user.getFullRef() + " PART " + (**it).getName();
// 				if (this->_params.size() > 1)
// 					this->_response += " :" + this->_params[1];
// 				this->_response += "\r\n";
// 				this->_respondChannel(*it);
// 				// Delete channel if this is the last user leaving
// 				if (!(**it).getUsers().size())
// 				{
// 					delete (*it);
// 					this->_server.rmChannel(*it);
// 				}
// 			}
// 		}
// 	}
// }
