#include "Message.hpp"

static void	parseParams(std::string &param, std::vector<std::string> &names, std::vector<std::string> &passes)
{
	size_t		space = param.find(' ');
	size_t		comma = 0;
	std::string	namesParam;
	std::string	passesParam;

	if (space != std::string::npos)
	{
		namesParam = param.substr(0, space);
		passesParam = param.substr(space + 1);
	}
	else
	{
		namesParam = param;
		passesParam = "";
	}

	while (comma = namesParam.find(',') != std::string::npos)
	{
		names.push_back(namesParam.substr(0, comma));
		namesParam.erase(0, comma + 1);
	}
	comma = 0;
	while (comma = passesParam.find(',') != std::string::npos)
	{
		passes.push_back(passesParam.substr(0, comma));
		passesParam.erase(0, comma + 1);
	}
}

void	Message::_JOIN(void)
{
	std::vector<std::string>	names;
	std::vector<std::string>	passes;

	if (this->_param == "")
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}

	parseParams(this->_param, names, passes);
	
	for (size_t i = 0; i < names.size(); i++)
	{
		if (names[i][0] != '#' ||
			names[i][0] != '+' ||
			names[i][0] != '&' ||
			names[i][0] != '!'
			)
			// 461		ERR_NEEDMOREPARAMS
			this->_response = ":" + this->_server.getServerName() + " 461 " + this->_command + " :Not enough parameters\r\n";
			this->_respondUser();
			return ;
		
	}
}