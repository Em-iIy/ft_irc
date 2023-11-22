#pragma once

#include <iostream>

class Config{
private:
	std::string	_serverName;
	std::string	_hostName;
	std::string _date;
	std::string _version;

public:
	Config();
	~Config();

	const std::string	&getServerName(void) const;
	const std::string	&getHostName(void) const;
	const std::string	&getDate(void) const;
	const std::string	&getVersion(void) const;

	void				setServerName(const std::string &serverName);
	void				setHostName(const std::string &hostName);
	void				setDate(const std::string &date);
	void				setVersion(const std::string &version);
};