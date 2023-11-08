#include "Config.hpp"

Config::Config()
{
	this->_serverName = "default";
	this->_hostName = "default";
	this->_version = "0.1";
}

Config::~Config()
{
}

// Getters
const std::string	&Config::getServerName(void) const
{
	return (this->_serverName);
}

const std::string	&Config::getHostName(void) const
{
	return (this->_hostName);
}

const std::string	&Config::getVersion(void) const
{
	return (this->_version);
}



// Setters
void				Config::setServerName(const std::string &serverName)
{
	this->_serverName = serverName;
}

void				Config::setHostName(const std::string &hostName)
{
	this->_hostName = hostName;
}

void				Config::setVersion(const std::string &version)
{
	this->_version = version;
}
