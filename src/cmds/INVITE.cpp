#include "Message.hpp"
#include <set>

void	Message::_INVITE(void)
{
	if (this->_params.size() < 2)
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_user.getNickname() + " " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}

	User	*targetUser = this->_server.getUserFromNick(this->_params[0]);

	if (!targetUser)
	{
		// 401		ERR_NOSUCHNICK
		this->_response = ":" + this->_server.getServerName() + " 401 " + this->_user.getNickname() + " " + this->_params[0] + " :No such nick/channel\r\n";
		this->_respondUser();
		return ;
	}

	Channel	*targetChannel = this->_server.getChannel(this->_params[1]);

	if (!targetChannel || !targetChannel->isUser(&this->_user))
	{
		// 442		ERR_NOTONCHANNEL
		this->_response = ":" + this->_server.getServerName() + " 442 " + this->_user.getNickname() + " " + this->_params[1] + " :You're not on that channel\r\n";
		this->_respondUser();
		return ;
	}
	if (targetChannel->checkMode(CMODE_I) && !targetChannel->isOper(&this->_user))
	{
		// 482		ERR_CHANOPRIVSNEEDED
		this->_response = ":" + this->_server.getServerName() + " 482 " + this->_user.getNickname() + " " + this->_params[1] + " :You're not channel operator\r\n";
		this->_respondUser();
		return ;
	}
	if (targetChannel->isUser(targetUser))
	{
		// 443		ERR_USERONCHANNEL
		this->_response = ":" + this->_server.getServerName() + " 443 " + this->_user.getNickname() + " " + targetUser->getNickname() + " " + targetChannel->getName() + " :is already on channel\r\n";
		this->_respondUser();
		return ;
	}
	if (targetChannel->checkMode(CMODE_I) && !targetChannel->isWhitelisted(targetUser))
		targetChannel->addWhitelist(targetUser);
	this->_response = ":" + this->_server.getServerName() + " 341 " + this->_user.getNickname() + " " + targetUser->getNickname() + " " + targetChannel->getName() + "\r\n";
	this->_respondUser();
	this->_response = ":" + this->_user.getFullRef() + " INVITE " + targetUser->getNickname() + " " + targetChannel->getName() + "\r\n";
	this->_respondTargetUser(targetUser);
}