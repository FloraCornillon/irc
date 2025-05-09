#include "../includes/Server.hpp"
#include "../includes/utils.hpp"


void Server::nick_command(const std::string& command, int fd) {
	std::istringstream stream(command);
	std::string commandType, nickName;

	stream >> commandType;
	stream >> nickName;
	if (nickName.empty())
	{
		sendError(fd, "", ERR_NEEDMOREPARAMS);
		return ;
	}
	if (!userAlreadyRegistered(nickName))
	{
		_clients[fd]->setNickName(nickName);
		std::cout << "nickname " << _clients[fd]->getNickName() << " nickname" << std::endl;
		return;
	}
	sendError(fd, nickName, ERR_NICKNAMEINUSE);
	return ;
}
