#include "Message.hpp"

void	Message::_AWAY(void)
{
	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
	if (this->_params.size() != 0)
		this->_user.setAwayMsg(this->_params[0]);
	else
		this->_user.setAwayMsg("");
	if (this->_user.checkMode(UMODE_A) == true)
	{
		this->_user.rmMode(UMODE_A);
		// 305		RPL_UNAWAY
		this->_response = ":" + this->_server.getServerName() + " 305 " + this->_user.getNickname() + " :You are no longer marked as being away\r\n";
		this->_respondUser();
		return ;
	}
	this->_user.addMode(UMODE_A);
	// 306		RPL_NOWAWAY
	this->_response = ":" + this->_server.getServerName() + " 306 " + this->_user.getNickname() + " :You have been marked as being away\r\n";
	this->_respondUser();
}
