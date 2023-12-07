#include "Message.hpp"

// Sends Channel info to the user when joining the channel
// !!! Needs to broadcast to the channel the the user has joined
void	Message::_welcomeChannel(Channel *channel)
{
	// 332		RPL_TOPIC
	if (channel->getTopic() != "")
	{
		this->_response = ":" + this->_server.getServerName() + " 332 " + this->_user.getNickname() + " " + channel->getName() + " :" + channel->getTopic() + "\r\n";
		this->_respondUser();
	}
	// 353		RPL_NAMEREPLY
	this->_response = ":" + this->_server.getServerName() + " 353 =" + channel->getName() + " :";
	for (std::list<User *>::iterator it = channel->getUsers().begin(); it != channel->getUsers().end(); ++it)
	{
		// Create extra response if list of users exceeds max message size
		if (this->_response.length() > MSG_BUFFERSIZE - 16)
		{
			this->_response.pop_back();
			this->_response += "\r\n";
			this->_respondUser();
			this->_response = ":" + this->_server.getServerName() + " 353 =" + channel->getName() + " :";
		}
		if (channel->isOper(*it))
			this->_response += "@";
		else
			this->_response += "=";
		this->_response += (*it)->getNickname() + " ";
	}
	this->_response.pop_back();
	this->_response += "\r\n";
	this->_respondUser();
	// 366		RPL_ENDOFNAMES
	this->_response = ":" + this->_server.getServerName() = " 366 " + channel->getName() + " :End of NAMES list\r\n";
	this->_respondUser();
}


void	Message::_JOIN(void)
{
	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
	if (!this->_params.size())
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}

	std::list<Channel *>		&channels = this->_server.getChannels();
	std::vector<std::string>	passes;
	std::string					pass;
	bool						channelExists;

	// adds requested channel names and passwords to their respective vectors
	std::vector<std::string>	names = parseParamByComma(this->_params[0]);
	if (this->_params.size() > 1)
		passes = parseParamByComma(this->_params[1]);

	for (size_t i = 0; i < names.size(); i++)
	{
		if (!isChannel(names[i]))
		{
			// 403		ERR_NOSUCHCHANNEL
			this->_response = ":" + this->_server.getServerName() + " 403 " + names[i] + " :No such channel\r\n";
			this->_respondUser();
			return ;
		}
		// set/reset channel existance
		channelExists = false;
		for (std::list<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			Channel *channel = *it;
			// if channel exists, attemt to add them to it
			if (channel->getName() == names[i])
			{
				channelExists = true;
				// Password iterator must be the same as the channels' in their respective vectors, else try without one
				pass = "";
				if (i < passes.size())
					pass = passes[i];
				try
				{
					channel->addUser(&(this->_user), pass);
					this->_welcomeChannel(channel);
				}
				catch(...)
				{
					// 475		ERR_BADCHANNELKEY
					this->_response = ":" + this->_server.getServerName() + " 475 " + names[i] + " :Cannot join channel (+k)\r\n";
					this->_respondUser();
				}
			}
		}
		// if the channel with that name doesn't exist, create one
		if (!channelExists)
		{
			pass = "";
			if (i < passes.size())
				pass = passes[i];
			Channel *channel = this->_server.addChannel(names[i], pass, &(this->_user));
			this->_welcomeChannel(channel);
		}
	}
	this->_server.printChannels();
}