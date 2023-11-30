#include "Message.hpp"

static std::vector<std::string>	parseParam(std::string param)
{
	size_t						comma = 0;
	std::vector<std::string>	channels;

	if (param.find(',') == std::string::npos)
		channels.push_back(param);
	else
	{
		while ((comma = param.find(',')) != std::string::npos)
		{
			channels.push_back(param.substr(0, comma));
			param.erase(0, comma + 1);
		}
	}
	return (channels);
}

void	Message::_PART(void)
{
	if (!this->_params.size())
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}

	std::vector<std::string>	channelsToPart = parseParam(this->_params[0]);
	std::list<Channel>			allChannels = this->_server.getChannels();

	for (size_t i = 0; i < channelsToPart.size(); i++)
	{
		for (std::list<Channel>::iterator it = allChannels.begin(); it != allChannels.end(); ++it)
		{
			if (channelsToPart[i] == (*it).getName())
			{
				if (!(*it).isUser(&(this->_user)))
				{
					// 442		ERR_NOTONCHANNEL
					this->_response = ":" + this->_server.getServerName() + " 442 " + (*it).getName() + " :You're not on that channel\r\n";
					this->_respondUser();
					break ;
				}
			}
		}
	}
}