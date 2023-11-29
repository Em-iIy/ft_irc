#include "Message.hpp"

void	Message::_USER_MODE(std::string &target, std::string &mode)
{
	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
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
		if (mode.length() != 2)
			throw std::exception();
		umode_t	inMode = cToUmode(mode[1]);
		if (inMode == UMODE_NONE)
			throw std::exception();
		
		// Check whether to add or remove the mode
		if (mode[0] == '+')
		{
			// ignore +o +O +a
			if (inMode != UMODE_LO &&
				inMode != UMODE_UO &&
				inMode != UMODE_A)
				this->_user.addMode(inMode);
		}
		else if (mode[0] == '-')
		{
			// ignore -a -r
			if (inMode != UMODE_R &&
				inMode != UMODE_A)
				this->_user.rmMode(inMode);
		}
		else
			throw std::exception();
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
	std::cout << target << " " << mode << " Channels not yet implemented >:(" << std::endl;
}


void	Message::_MODE(void)
{
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