#pragma once

#include "types.hpp"
#include <poll.h>
#include <string>

void		rmCRLF(std::string &str);
void		msgLimitSize(std::string &str);
pollfd		initPFD(sockfd_t fd);
bool		isChannel(const std::string &str);
umode_e		cToUmode(char c);
std::string	umodeToStr(const umode_t mode);
std::string	date(void);
std::string	time(void);
