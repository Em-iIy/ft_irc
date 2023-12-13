#include "Message.hpp"

void	Message::_USER_MODE(std::string &target, std::string &mode)
{
	if (target != this->_user.getNickname())
	{
		// 502		ERR_USERSDONTMATCH
		this->_response = ":" + this->_server.getServerName() + " 502 " + this->_user.getNickname() + " :Cannot change mode for other users\r\n";
		this->_respondUser();
		return ;
	}
	if (mode.length() == 0)
	{
		// 221		RPL_UMODEIS
		this->_response = ":" + this->_server.getServerName() + " 221 " + this->_user.getNickname() + " " + umodeToStr(this->_user.getMode()) + "\r\n";
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
		this->_response = ":" + this->_server.getServerName() + " 221 " + this->_user.getNickname() + " " + umodeToStr(this->_user.getMode()) + "\r\n";
		this->_respondUser();
	}
	catch(...)
	{
		// 501		ERR_UMODEUNKNOWNFLAG
		this->_response = ":" + this->_server.getServerName() + " 501 " + this->_user.getNickname() + " :Unknown MODE flag\r\n";
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
		this->_response = ":" + this->_server.getServerName() + " 403 " + this->_user.getNickname() + " " + target + " :No such channel\r\n";
		this->_respondUser();
		return ;
	}
	if (!targetChannel->isUser(&this->_user))
	{
		// 442		ERR_NOTONCHANNEL
		this->_response = ":" + this->_server.getServerName() + " 442 " + this->_user.getNickname() + " " + target + " :You're not on that channel\r\n";
		this->_respondUser();
		return ;
	}
	if (mode.empty())
	{
		// 324		RPL_CHANNELMODEIS
		this->_response = ":" + this->_server.getServerName() + " 324 " + this->_user.getNickname() + " " + target + " " + cmodeToStr(targetChannel->getMode());
		if (targetChannel->checkMode(CMODE_K))
			this->_response += " " + targetChannel->getPass();
		if (targetChannel->checkMode(CMODE_L))
			this->_response += " " + std::to_string(targetChannel->getLimit());
		this->_response += "\r\n";
		this->_respondUser();
		return ;
	}
	if (!targetChannel->isOper(&this->_user))
	{
		// 482		ERR_CHANOPRIVSNEEDED
		this->_response = ":" + this->_server.getServerName() + " 482 " + this->_user.getNickname() + " " + target + " :You're not channel operator\r\n";
		this->_respondUser();
		return ;
	}
	cmode_t				tempMode = targetChannel->getMode();
	std::list<User *>	tempOpers = targetChannel->getOpers();
	std::string 		tempPass = targetChannel->getPass();
	int					count = 0;
	try
	{
		for (std::vector<std::string>::iterator it = this->_params.begin() + 1; it != this->_params.end(); ++it)
		{
			bool		modeBool; // True for adding, false for removing
			cmode_t		inMode;
			size_t		i = 0;
			std::string	flags = *it;

			while (i < flags.size())
			{
				if (flags[i] != '+' && flags[i] != '-')
					throw std::runtime_error(":" + this->_server.getServerName() + " 472 " + this->_user.getNickname() + " " + flags[i] + " :is unknown mode char to me for " + targetChannel->getName() + "\r\n");
				if (flags[i] == '+')
					modeBool = true;
				else
					modeBool = false;
				i++;
				while (flags[i] != '+' && flags[i] != '-' && flags[i])
				{
					inMode = cToCmode(flags[i]);
					if (inMode == CMODE_NONE)
						throw std::runtime_error(":" + this->_server.getServerName() + " 472 " + this->_user.getNickname() + " " + flags[i] + " :is unknown mode char to me for " + targetChannel->getName() + "\r\n");
					if (modeBool)
					{
						if (inMode == CMODE_K && count < 3)
						{
							it++;
							count++;
							if (it == this->_params.end())
								throw std::runtime_error(":" + this->_server.getServerName() + " 461 " + this->_user.getNickname() + " " + this->_command + " :Not enough parameters\r\n");
							if (tempMode & CMODE_K)
								throw std::runtime_error(":" + this->_server.getServerName() + " 467 " + targetChannel->getName() + " :Channel key already set\r\n");
							tempPass = *it;
							tempMode |= CMODE_K;
						}
						else if (inMode == CMODE_O && count < 3)
						{
							it++;
							count++;
							if (it == this->_params.end())
								throw std::runtime_error(":" + this->_server.getServerName() + " 461 " + this->_user.getNickname() + " " + this->_command + " :Not enough parameters\r\n");
							User *targetUser = targetChannel->getUserFromNick(*it);
							if (!targetChannel->isUser(targetUser))
								throw std::runtime_error(":" + this->_server.getServerName() + " 441 " + *it + " " + targetChannel->getName() + " :They aren't on that channel\r\n");
							if (std::find(tempOpers.begin(), tempOpers.end(), targetUser) == tempOpers.end())
								tempOpers.push_back(targetUser);
						}
						else if (inMode == CMODE_L && count < 3)
						{
							it++;
							count++;
							if (it == this->_params.end())
								throw std::runtime_error(":" + this->_server.getServerName() + " 461 " + this->_user.getNickname() + " " + this->_command + " :Not enough parameters\r\n");
							int limit = stoi(*it);
							if (limit > 0xffff || limit < 0)
								limit = 0xffff;
							tempMode &= ~0xffff;
							tempMode |= limit;
							tempMode |= CMODE_L;
						}
						else if (inMode == CMODE_I)
						{
							targetChannel->makeWhitelist();
							tempMode |= CMODE_I;
						}
						else if (inMode == CMODE_T)
							tempMode |= CMODE_T;
					}
					else
					{
						if (inMode == CMODE_O && count < 3)
						{
							it++;
							count++;
							if (it == this->_params.end())
								throw std::runtime_error(":" + this->_server.getServerName() + " 461 " + this->_user.getNickname() + " " + this->_command + " :Not enough parameters\r\n");
							User *targetUser = targetChannel->getUserFromNick(*it);
							if (!targetChannel->isUser(targetUser))
								throw std::runtime_error(":" + this->_server.getServerName() + " 441 " + *it + " " + targetChannel->getName() + " :They aren't on that channel\r\n");
							tempOpers.remove(targetUser);
						}
						else
							tempMode &= ~inMode;
					}
					i++;
				}
			}
		}
	}
	catch(std::runtime_error &e)
	{
		this->_response = e.what();
		this->_respondUser();
		return ;
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return ;
	}
	targetChannel->setMode(tempMode);
	targetChannel->setPass(tempPass);
	targetChannel->setOpers(tempOpers);
	this->_response = ":" + this->_user.getFullRef() + " MODE";
	for (size_t i = 0; i < this->_params.size(); i++)
		this->_response += " " + this->_params[i];
	this->_response += "\r\n";
	this->_respondChannel(targetChannel);
}

void	Message::_MODE(void)
{
	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
	if (this->_params.size() == 0)
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_user.getNickname() + " " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}
	std::string	&target = this->_params[0];
	std::string mode;
	if (this->_params.size() > 1)
		mode = this->_params[1];
	// Check whether a channel or user mode is being changed
	if (isChannel(target))
		this->_CHANNEL_MODE(target, mode);
	else
		this->_USER_MODE(target, mode);
}
