#include "Message.hpp"

static void	parseParams(std::vector<std::string> &params, std::vector<std::string> &names, std::vector<std::string> &passes)
{
	size_t		comma = 0;
	std::string	namesParam = params[0];
	std::string	passesParam = "";

	if (params.size() > 1)
		passesParam = params[1];
	if (namesParam.find(',') == std::string::npos)
		names.push_back(namesParam);
	while ((comma = namesParam.find(',')) != std::string::npos)
	{
		names.push_back(namesParam.substr(0, comma));
		namesParam.erase(0, comma + 1);
	}
	comma = 0;
	if (passesParam.find(',') == std::string::npos)
		passes.push_back(passesParam);
	while ((comma = passesParam.find(',')) != std::string::npos)
	{
		passes.push_back(passesParam.substr(0, comma));
		passesParam.erase(0, comma + 1);
	}
}

void	Message::_JOIN(void)
{
	std::list<Channel *>		&channels = this->_server.getChannels();
	std::vector<std::string>	names;
	std::vector<std::string>	passes;
	std::string					pass;
	bool						channelExists;

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
	// adds requested channel names and passwords to their respective vectors
	parseParams(this->_params, names, passes);

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
			// if channel exists, attemt to add them to it
			if ((**it).getName() == names[i])
			{
				channelExists = true;
				// Password iterator must be the same as the channels' in their respective vectors, else try without one
				pass = "";
				if (i < passes.size())
					pass = passes[i];
				try
				{
					(**it).addUser(&(this->_user), pass);
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
			this->_server.addChannel(names[i], pass, &(this->_user));
		}
	}
	this->_server.printChannels();
}