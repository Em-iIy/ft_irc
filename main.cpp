#include <iostream>
#include <vector>
#include <exception>
#include <Server.hpp>
#include <Socket.hpp>
#include <poll.h>
#include <errno.h>

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "usage: ircserv [port] [password]" << std::endl;
		return (1);
	}
	try
	{
		Server serv(argv[1], argv[2]);
		serv.Start();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}