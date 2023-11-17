#pragma once

#include "types.hpp"
#include <poll.h>
#include <string>

void	rmCRLF(std::string &str);
pollfd	initPFD(sockfd_t fd);
