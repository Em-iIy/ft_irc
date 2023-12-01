#include "Message.hpp"

void	Message::_PART(void)
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

	std::vector<std::string>	channelsToPart = parseParamByComma(this->_params[0]);
	std::list<Channel *>		allChannels = this->_server.getChannels();

	for (size_t i = 0; i < channelsToPart.size(); i++)
	{
		for (std::list<Channel *>::iterator it = allChannels.begin(); it != allChannels.end(); ++it)
		{
			if (channelsToPart[i] == (**it).getName())
			{
				if (!(**it).isUser(&(this->_user)))
				{
					// 442		ERR_NOTONCHANNEL
					this->_response = ":" + this->_server.getServerName() + " 442 " + (**it).getName() + " :You're not on that channel\r\n";
					this->_respondUser();
					break ;
				}
				// If operator leaves, remove them as such from the channel as well
				if ((**it).isOper(&(this->_user)))
					(**it).rmOper(&(this->_user));
				(**it).rmUser(&(this->_user));

				// Delete channel if this is the last user leaving
				if (!(**it).getUsers().size())
				{
					delete (*it);
					this->_server.rmChannel(*it);
				}
			}
		}
	}
	this->_server.printChannels();
}