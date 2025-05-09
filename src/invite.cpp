#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"
#include "../includes/utils.hpp"


void Server::invite_command(const std::string &command, int fd) {
	std::istringstream stream(command);
	std::string commandType, nickName, channel;
	stream >> commandType; //INVITE
	stream >> nickName;
	stream >> channel;

	// check params
	if (channel.empty() || nickName.empty()) {
		sendError(fd, "", ERR_NEEDMOREPARAMS); // Pas assez de paramètres
		return;
	}

	//check channel exist
		if (_channels.find(channel) == _channels.end())
		{
			return (sendError(fd, "", ERR_NOSUCHCHANNEL));
		}
	// check channel mode and is operator
	if (!(getChannel(channel)->isMode('i') && getChannel(channel)->isOperator(fd)))
		return sendError(fd, "", ERR_CHANOPRIVSNEEDED);
	// check user exist
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->getNickName() == nickName) {
			sendToClient(it->second->getClient_fd(), "you are invited by ...");//!check msg
			getChannel(channel)->inviteUser(it->second);
			return (sendToClient(fd, RPL_INVITING));
		}
	}
		return sendError(fd, "", ERR_NOSUCHCHANNEL);
}