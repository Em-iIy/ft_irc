#pragma once
#include <vector>
#include "poll.h"

// Classes
class Server;
class Config;
class Socket;
class Log;
class User;
class Message;

// Enums
enum status_e {
	STAT_NEW		= 0b000000,
	STAT_CAP_START	= 0b000001,
	STAT_CAP_END	= 0b000010,
	STAT_REG		= 0b000100,
	STAT_REG_PASS	= 0b001000,
	STAT_REG_NICK	= 0b010000,
	STAT_REG_USER	= 0b100000
};


// Types
typedef int sockfd_t;
typedef int status_t;
typedef std::vector<pollfd>::iterator pollfdIt;

// Macros
#define DEBUG(x) std::cout << "User #" << x << " " << __func__ << " " << __FILE__ << ":" << __LINE__ << std::endl
