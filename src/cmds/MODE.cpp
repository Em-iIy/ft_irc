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
	if (!this->_server.hasChannel(target))
	{
		// 403		ERR_NOSUCHCHANNEL
		this->_response = ":" + this->_server.getServerName() + " 403 " + target + " :No such channel\r\n";
		this->_respondUser();
		return ;
	}
	if (!this->_server.getChannel(target)->isUser(&this->_user))
	{
		// 442		ERR_NOTONCHANNEL
		this->_response = ":" + this->_server.getServerName() + " 442 " + target + " :You're not on that channel\r\n";
		this->_respondUser();
		return ;
	}
	if (!this->_server.getChannel(target)->isOper(&this->_user))
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

	// WORK IN PROGRESS: ->figure out if it's + or - (last one before a different character)
	//					 ->do that to every character it finds afterwards (error response if unknown mode)
	//					 ->in case a setting is needed, get the next setting from the vector

	size_t		i = 1;
	bool		modeBool = false; // True for adding, false for removing
	std::string	modeParams = "";

	while (i < this->_params.size())
	{
		if (this->_params[i][0] == '+')
			modeBool = true;
		else if (this->_params[i][0] != '-')
			return ;
		if ((i + 1) < this->_params.size())
		{
			modeParams = this->_params[i + 1];
			i++;
		}
		i++;
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