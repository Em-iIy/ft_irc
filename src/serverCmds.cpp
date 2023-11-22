#include "Server.hpp"
#include "User.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <exception>

// Prints out a list of all the nicknames in use
static void	nicks(Server &serv)
{
	std::vector<std::string> nicks = serv.getNicknames();

	std::cout << "Nicknames:" << std::endl;
	for (std::vector<std::string>::iterator it = nicks.begin(); it != nicks.end(); it++)
		std::cout << *it << std::endl;
}

// Prints out a list of all the connected users with their nickname (if any)
static void	users(Server &serv)
{
	std::map<sockfd_t, User &> users = serv.getUsers();

	std::cout << "Users:" << std::endl;
	for (std::map<sockfd_t, User &>::iterator it = users.begin(); it != users.end(); it++)
		std::cout << "User #" << it->first << " - " << it->second.getNickname() << std::endl;
}

// Prints out the information of the given user. Either by #fd or nickname
static void	user(std::string &param, Server &serv)
{
	std::map<sockfd_t, User &> users = serv.getUsers();

	std::cout << std::endl;
	try
	{
		if (param[0] == '#')
		{
			int	fd = std::stoi(param.c_str() + 1);
			User	&usr = users.at(fd);
			std::cout << usr << std::endl;
			return ;
		}

		std::map<sockfd_t, User &>::iterator it = users.begin();
		for (; it != users.end(); it++)
		{
			if (it->second.getNickname() == param)
			{
				User	&usr = it->second;
				std::cout << usr << std::endl;
				return ;
			}
		}
		if (it == users.end())
			throw std::runtime_error("Nickname not found");
	}
	catch(const std::exception& e)
	{
		std::cerr << "Couldn't find user: " << param << "\treason: " << e.what() << '\n';
	}
}

// Broadcasts a server message to all connected users
static void	broadcast(std::string &param, Server &serv)
{
	std::string msg;

	msg = ":" + serv.getServerName() + " NOTICE all :" + param + "\n";
	serv.broadcastMsg(msg);
}

void	serverCmd(std::string cmd, Server &serv)
{
	rmCRLF(cmd);
	size_t space = cmd.find(' ');
	std::string command;
	std::string param;

	serv.log("Server", cmd);
	// Commands without parameters
	if (space == std::string::npos)
	{
		if (cmd == "USERS")
		{
			users(serv);
			return ;
		}
		if (cmd == "NICKS")
		{
			nicks(serv);
			return ;
		}
		if (cmd == "QUIT")
		{
			serv.Stop();
			return ;
		}
		return ;
	}

	command = cmd.substr(0, space);
	param = cmd.substr(space + 1);
	// Commands with parameters
	if (command == "USER")
	{
		user(param, serv);
		return ;
	}
	if (command == "BC")
	{
		broadcast(param, serv);
		return ;
	}
}