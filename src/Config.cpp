#include "Config.hpp"
#include <limits.h>
#include <unistd.h>

Config::Config()
{
	#ifdef HOST_NAME
		this->_hostName = HOST_NAME;
	#else
	/* can't do this because it's not an "allowed" function (it's the same thing essentially)
		char hostname[HOST_NAME_MAX];
		gethostname(hostname, HOST_NAME_MAX);
		this->_hostName = hostname;
	*/
		this->_hostName = "default";
	#endif
	this->_serverName = "cc.tv"; // Codam Chat tv :)
	this->_version = "0.2";
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
