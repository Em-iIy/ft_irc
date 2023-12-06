#include "Message.hpp"

void	Message::_USER_MODE(std::string &target, std::string &mode)
{
	if (target != this->_user.getNickname())
	{
		// 502		ERR_USERSDONTMATCH
		this->_response = ":" + this->_server.getServerName() + " 502 :Cannot change mode for other users\r\n";
		this->_respondUser();
		return ;
	}
	if (mode.length() == 0)
	{
		// 221		RPL_UMODEIS
		this->_response = ":" + this->_server.getServerName() + " 221 " + target + " :" + umodeToStr(this->_user.getMode()) + "\r\n";
		this->_respondUser();
		return ;
	}
	try
	{
		size_t	i = 0;
		bool	modeBool; // True for adding, false for removing
		umode_t	inMode;
		while (i < mode.size())
		{
			if (mode[i] != '+' && mode[i] != '-')
				throw std::exception();
			while (mode[i] == '+' || mode[i] == '-')
				i++;
			// modeBool = (mode[i - 1] == '+') ? true : false; Em finds this too hard to read :(
			if (mode[i - 1] == '+')
				modeBool = true;
			else
				modeBool = false;
			while (mode[i] != '+' && mode[i] != '-' && mode[i])
			{
				inMode = cToUmode(mode[i]);
				if (inMode == UMODE_NONE)
					throw std::exception();
				if (modeBool)
				{
					// ignore +o +O +a
					if (inMode != UMODE_LO &&
						inMode != UMODE_UO &&
						inMode != UMODE_A)
						this->_user.addMode(inMode);
				}
				else
				{
					// ignore -a -r
					if (inMode != UMODE_R &&
						inMode != UMODE_A)
						this->_user.rmMode(inMode);
				}
				i++;
			}
		}
		// 221		RPL_UMODEIS
		this->_response = ":" + this->_server.getServerName() + " 221 " + target + " :" + umodeToStr(this->_user.getMode()) + "\r\n";
		this->_respondUser();
	}
	catch(...)
	{
		// 501		ERR_UMODEUNKNOWNFLAG
		this->_response = ":" + this->_server.getServerName() + " 501 :Unknown MODE flag\r\n";
		this->_respondUser();
		return ;
	}
}

void	Message::_CHANNEL_MODE(std::string &target, std::string &mode)
{
	Channel *targetChannel = this->_server.getChannel(target);

	if (!this->_server.hasChannel(target))
	{
		// 403		ERR_NOSUCHCHANNEL
		this->_response = ":" + this->_server.getServerName() + " 403 " + target + " :No such channel\r\n";
		this->_respondUser();
		return ;
	}
	if (!targetChannel->isUser(&this->_user))
	{
		// 442		ERR_NOTONCHANNEL
		this->_response = ":" + this->_server.getServerName() + " 442 " + target + " :You're not on that channel\r\n";
		this->_respondUser();
		return ;
	}
	if (!targetChannel->isOper(&this->_user))
	{
		// 482		ERR_CHANOPRIVSNEEDED
		this->_response = ":" + this->_server.getServerName() + " 482 " + target + " :You're not channel operator\r\n";
		this->_respondUser();
		return ;
	}
	if (mode.length() == 0)
	{
		// 324		RPL_CHANNELMODEIS
		this->_response = ":" + this->_server.getServerName() + " 324 " + target + " :" + cmodeToStr(this->_server.getChannel(target)->getMode()) + "\r\n";
		this->_respondUser();
		return ;
	}
	try
	{
		for (std::vector<std::string>::iterator it = this->_params.begin() + 1; it != this->_params.end(); ++it)
		{
			bool	modeBool; // True for adding, false for removing
			cmode_t	inMode;
			size_t	i = 0;
			while (i < it->size())
			{
				if ((*it)[i] != '+' && (*it)[i] != '-')
					throw std::runtime_error(":" + this->_server.getServerName() + " 472 :Unknown MODE flag\r\n");
				while ((*it)[i] == '+' || (*it)[i] == '-')
					i++;
				if ((*it)[i - 1] == '+')
					modeBool = true;
				else
					modeBool = false;
				while ((*it)[i] != '+' && (*it)[i] != '-' && (*it)[i])
				{
					inMode = cToCmode((*it)[i]);
					if (inMode == CMODE_NONE)
						throw std::runtime_error(":" + this->_server.getServerName() + " 472 " + (*it)[i] + " :is unknown mode char to me for " + targetChannel->getName() + "\r\n");
					if (modeBool)
					{
						if ((inMode == CMODE_K) && (it + 1 != this->_params.end()))
						{
							it++;
							if (!targetChannel->setPass(*(it + 1)))
								throw std::runtime_error(":" + targetChannel->getName() + " 467 :Channel key already set\r\n");
						}
					}
					else
					{
						continue; // WORK IN PROGRESS
					}
					i++;
				}
			}
		}
	}
	catch(std::exception &e)
	{
		this->_response = e.what();
		this->_respondUser();
		return ;
	}
}

void	Message::_MODE(void)
{
	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
	if (this->_params.size() == 0)
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}
	std::string	&target = this->_params[0];
	std::string mode = "";
	if (this->_params.size() > 1)
		mode = this->_params[1];
	// Check whether a channel or user mode is being changed
	if (isChannel(target))
		this->_CHANNEL_MODE(target, mode);
	else
		this->_USER_MODE(target, mode);
}