#include "Message.hpp"

void	Message::_TOPIC(void)
{
	Channel *target;
	if (this->_user.getRegistered() == false)
		return ;
	if (!this->_params.size())
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_user.getNickname() + " " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}
	target = this->_user.getChannel(this->_params[0]);
	if (!target)
	{
		// 442		ERR_NOTONCHANNEL
		this->_response = ":" + this->_server.getServerName() + " 442 " + this->_user.getNickname() + " " + this->_params[0] + " :You're not on that channel\r\n";
		this->_respondUser();
		return ;
	}
	if (this->_params.size() == 1)
	{
		const std::string &topic = target->getTopic();
		if (topic == "")
		{
			// 331		RPL_NOTOPIC
			this->_response = ":" + this->_server.getServerName() + " 331 " + this->_user.getNickname() + " " + target->getName() + " :No topic is set\r\n";
			this->_respondUser();
		}
		else
		{
			// 332		RPL_TOPIC
			this->_response = ":" + this->_server.getServerName() + " 332 " + this->_user.getNickname() + " " + target->getName() + " :" + topic + "\r\n";
			this->_respondUser();
		}
		return ;
	}
	if (target->checkMode(CMODE_T) && target->isOper(&this->_user) == false)
	{
		// 482		ERR_CHANOPRIVSNEEDED
		this->_response = ":" + this->_server.getServerName() + " 482 " + this->_user.getNickname() + " " + target->getName() + " :You're not channel operator\r\n";
		this->_respondUser();
		return ;
	}
	target->setTopic(this->_params[1]);
	// 332		RPL_TOPIC
	this->_response = ":" + this->_user.getFullRef() + " TOPIC " + target->getName() + " :" + this->_params[1] + "\r\n";
	this->_respondChannel(target);
}
