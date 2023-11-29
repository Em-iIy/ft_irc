#include "Message.hpp"

static void	parseParams(std::vector<std::string> &params, std::vector<std::string> &names, std::vector<std::string> &passes)
{
	size_t		comma = 0;
	std::string	namesParam = params[0];
	std::string	passesParam = "";
	if (params.size() > 1)
		passesParam = params[1];

	while ((comma = namesParam.find(',')) != std::string::npos)
	{
		names.push_back(namesParam.substr(0, comma));
		namesParam.erase(0, comma + 1);
	}
	comma = 0;
	while ((comma = passesParam.find(',')) != std::string::npos)
	{
		passes.push_back(passesParam.substr(0, comma));
		passesParam.erase(0, comma + 1);
	}
}

void	Message::_JOIN(void)
{
	std::vector<std::string>	names;
	std::vector<std::string>	passes;
	std::string					pass;

	if (!this->_params.size())
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}

	parseParams(this->_params, names, passes);
	
	for (size_t i = 0; i < names.size(); i++)
	{
		if (isChannel(names[i]))
		{
			// 403		ERR_NOSUCHCHANNEL
			this->_response = ":" + this->_server.getServerName() + " 403 " + names[i] + " :No such channel\r\n";
			this->_respondUser();
			return ;
		}
	
		std::list<Channel>	&channels = this->_server.getChannels();
		for (std::list<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			if ((*it).getName() == names[i])
			{
				pass = "";
				if (i < passes.size())
					pass = passes[i];
				try
				{
					(*it).addUser(&(this->_user), pass);
					return ;
				}
				catch(const std::exception& e)
				{
					// 475		ERR_BADCHANNELKEY
					this->_response = ":" + this->_server.getServerName() + " 475 " + names[i] + " :Cannot join channel (+k)\r\n";
					this->_respondUser();
					return ;
				}
			}
		}
		pass = "";
		if (i < passes.size())
			pass = passes[i];
		this->_server.addChannel(names[i], pass, &(this->_user));
	}
}