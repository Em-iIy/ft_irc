#include "Message.hpp"

void	Message::_makeWHOResponse(User *user)
{
	if (user->checkMode(UMODE_I) == true)
		return ;
	this->_response = ":" + this->_server.getServerName() + " 352 " + this->_user.getNickname() + " ";
	if (this->_params.size() && isChannel(this->_params[0]))
		this->_response += this->_params[0] + " ";
	else
		this->_response += "* ";
	this->_response += user->getUsername() + " " + user->getHostname() + " " + user->getServername() + " " + user->getNickname() + " ";
	if (user->checkMode(UMODE_A))
		this->_response += "G";
	else
		this->_response += "H";
	this->_response += " :0 " + user->getRealname() + "\r\n";
	this->_respondUser();
}

void	Message::_WHO(void)
{
	// Must be registered to use this command
	if (this->_user.getRegistered() == false)
		return ;
	if (this->_params.empty())
	{
		for (std::map<sockfd_t, User &>::iterator it = this->_server.getUsers().begin(); it != this->_server.getUsers().end(); ++it)
			this->_makeWHOResponse(&(*it).second);
		this->_response = ":" + this->_server.getServerName() + " 315 " + this->_user.getNickname() + " :End of WHO list\r\n";
		this->_respondUser();
		return ;
	}
	std::string &target = this->_params[0];
	if (isChannel(target))
	{
		Channel *channel = this->_server.getChannel(target);
		if (channel)
		{
			std::list<User *>	users = channel->getUsers();
		
			for (std::list<User *>::iterator it = users.begin(); it != users.end(); ++it)
				this->_makeWHOResponse(*it);
		}
	}
	else
		this->_makeWHOResponse(this->_server.getUserFromNick(target));
	this->_response = ":" + this->_server.getServerName() + " 315 " + this->_user.getNickname() + target + " :End of WHO list\r\n";
	this->_respondUser();
	return ;
}