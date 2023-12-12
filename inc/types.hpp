#pragma once

#include <vector>
#include <poll.h>
#include <iostream>

// Classes
class Server;
class Config;
class Socket;
class Log;
class User;
class Message;
class Channel;

// Defines
#define MSG_BUFFERSIZE 512

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

enum	cmode_e {
	CMODE_NONE		= 0x000000,
	CMODE_I			= 0x010000,
	CMODE_T			= 0x020000,
	CMODE_K			= 0x040000,
	CMODE_O			= 0x080000,
	CMODE_L			= 0x100000,
};

enum	cmd_e {
	CMD_NOT_FOUND,
	CMD_AWAY,
	CMD_CAP,
	CMD_ISON,
	CMD_MODE,
	CMD_NICK,
	CMD_PASS,
	CMD_PING,
	CMD_PONG,
	CMD_PRIVMSG,
	CMD_QUIT,
	CMD_TOPIC,
	CMD_USER,
	CMD_VERSION,
	CMD_JOIN,
	CMD_WHOIS,
	CMD_PART,
	CMD_INVITE,
};

// Types
typedef int sockfd_t;
typedef int status_t;
typedef int umode_t;
typedef int	cmode_t;
typedef std::vector<pollfd>::iterator pollfdIt;

// Macros
#ifdef DEBUG_MODE
	#define DEBUG(x) std::cout << "User #" << x << " " << __func__ << " " << __FILE__ << ":" << __LINE__ << std::endl
#else
	#define DEBUG(x)
#endif
