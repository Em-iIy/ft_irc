#include "Message.hpp"

void	Message::_AWAY(void)
{
	if (this->_param != "")
	{
		if (this->_param[0] == ':')
			this->_param.erase(0, 1);
		this->_user.setAwayMsg(this->_param);
	}
	if (this->_user.checkMode(UMODE_A) == true)
	{
		this->_user.rmMode(UMODE_A);
		// 305		RPL_UNAWAY
		this->_response = ":" + this->_server.getHostName() + " 305 :You are no longer marked as being away\n";
		this->_respondUser();
		return ;
	}
	this->_user.addMode(UMODE_A);
	// 306		RPL_NOWAWAY
	this->_response = ":" + this->_server.getHostName() + " 306 :You have been marked as being away\n";
	this->_respondUser();
}