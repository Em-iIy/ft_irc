#pragma once

#include <vector>
#include <poll.h>

// Classes
class Server;
class Config;
class Socket;
class Log;
class User;
class Message;

// Enums
enum	status_e {
	STAT_NEW		= 0x00,
	STAT_CAP_START	= 0x01,
	STAT_CAP_END	= 0x02,
	STAT_REG		= 0x04,
	STAT_REG_PASS	= 0x08,
	STAT_REG_NICK	= 0x10,
	STAT_REG_USER	= 0x20,
};

enum	umode_e {
	UMODE_NONE		= 0x00,
	UMODE_A			= 0x01,
	UMODE_I			= 0x02,
	UMODE_W			= 0x04,
	UMODE_R			= 0x08,
	UMODE_LO		= 0x10,
	UMODE_UO		= 0x20,
	UMODE_S			= 0x40,
};

enum	cmd_e {
	CMD_NOT_FOUND,
	CMD_AWAY,
	CMD_CAP,
	CMD_MODE,
	CMD_NICK,
	CMD_PASS,
	CMD_PING,
	CMD_QUIT,
	CMD_USER,
	CMD_VERSION,
};

// Types
typedef int sockfd_t;
typedef int status_t;
typedef int umode_t;
typedef std::vector<pollfd>::iterator pollfdIt;

// Macros
#ifdef DEBUG_MODE
	#define DEBUG(x) std::cout << "User #" << x << " " << __func__ << " " << __FILE__ << ":" << __LINE__ << std::endl
#else
	#define DEBUG(x)
#endif
