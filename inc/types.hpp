#pragma once
#include <vector>
#include "poll.h"

// Classes
class Server;
class Config;
class Socket;
class User;
class Message;

// Types
typedef int sockfd_t;
typedef std::vector<pollfd>::iterator pollfdIt;

// Macros
#define DEBUG(x) std::cout << "User #" << x << " " << __func__ << " " << __FILE__ << ":" << __LINE__ << std::endl
