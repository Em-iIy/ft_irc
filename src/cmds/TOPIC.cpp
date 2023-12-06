#include "Message.hpp"

void	Message::_TOPIC(void)
{
	Channel *target;
	if (this->_user.getRegistered() == false)
		return ;
	if (!this->_params.size())
	{
		// 461		ERR_NEEDMOREPARAMS
		this->_response = ":" + this->_server.getServerName() + " 461 " + this->_command + " :Not enough parameters\r\n";
		this->_respondUser();
		return ;
	}
	target = this->_user.getChannel(this->_params[0]);
	if (!target)
	{
		// 442		ERR_NOTONCHANNEL
		this->_response = ":" + this->_server.getServerName() + " 442 " + target->getName() + " :You're not on that channel\r\n";
		this->_respondUser();
		return ;
	}
	if (this->_params.size() == 1)
	{
		const std::string &topic = target->getTopic();
		if (topic == "")
		{
			// 331		RPL_NOTOPIC
			this->_response = ":" + this->_server.getServerName() + " 331 " + target->getName() + " :No topic is set\r\n";
			this->_respondUser();
		}
		else
		{
			// 332		RPL_TOPIC
			this->_response = ":" + this->_server.getServerName() + " 332 " + target->getName() + " :" + topic + "\r\n";
			this->_respondUser();
			// 333		RPL_TOPICWHOTIME
			this->_response = ":" + this->_server.getServerName() + " 333 " + target->getName() + " " + target->getTopicSetBy() + " " + std::to_string(target->getTopicSetAt()) + "\r\n";
			this->_respondUser();
		}
		return ;
	}
	if (target->checkMode(CMODE_T) && target->isOper(&this->_user) == false)
	{
		// 482		ERR_CHANOPRIVSNEEDED
		this->_response = ":" + this->_server.getServerName() + " 482 " + target->getName() + " :You're not channel operator\r\n";
		this->_respondUser();
		return ;
	}
	target->setTopic(this->_params[1]);
	target->setTopicSetBy(this->_user.getNickname());
	target->setTopicSetAt(std::time(NULL));
	// 332		RPL_TOPIC
	this->_response = ":" + this->_server.getServerName() + " 332 " + target->getName() + " :" + this->_params[1] + "\r\n";
	this->_respondUser();
	// 333		RPL_TOPICWHOTIME
	this->_response = ":" + this->_server.getServerName() + " 333 " + target->getName() + " " + target->getTopicSetBy() + " " + std::to_string(target->getTopicSetAt()) + "\r\n";
	this->_respondUser();
}