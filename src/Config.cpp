#include "Config.hpp"
#include "utils.hpp"
#include <limits.h>
#include <unistd.h>


Config::Config()
{
	#ifdef HOST_NAME
		this->_hostName = HOST_NAME;
	#else
	/* // Can't do this because it's not an "allowed" function (it's the same thing essentially)
		char hostname[HOST_NAME_MAX];
		gethostname(hostname, HOST_NAME_MAX);
		this->_hostName = hostname;
	*/
		this->_hostName = "default";
	#endif
	this->_serverName = "cc.tv"; // Codam Chat tv :)
	this->_date = date();
	this->_version = "0.2";
	this->_comments = "This version number is arbitrary";
	#ifdef DEBUG_MODE
		this->_debug = DEBUG_MODE;
	#else
		this->_debug = "0";
	#endif
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

const std::string	&Config::getDate(void) const
{
	return (this->_date);
}

const std::string	&Config::getVersion(void) const
{
	return (this->_version);
}

const std::string	&Config::getDebug(void) const
{
	return (this->_debug);
}

const std::string	&Config::getComments(void) const
{
	return (this->_comments);
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

void				Config::setDate(const std::string &date)
{
	this->_date = date;
}

void				Config::setVersion(const std::string &version)
{
	this->_version = version;
}

void				Config::setDebug(const std::string &debug)
{
	this->_debug = debug;
}

void				Config::setComments(const std::string &comments)
{
	this->_comments = comments;
}
