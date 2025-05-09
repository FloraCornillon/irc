#include "../includes/Server.hpp"
#include "../includes/utils.hpp"


void Server::part_command(const std::string& command, int fd) {
	std::istringstream stream(command);
	std::string commandType, channelsPart, message;

	stream >> commandType;

	if (!std::getline(stream, channelsPart, ':')) {
		sendError(fd, "", ERR_UNKNOWNCOMMAND);
		return;
	}

	if (stream) {
		std::getline(stream, message);
	}

	std::vector<std::string> channels = split(channelsPart, ',');
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
		*it = trim(*it, " \t\r\n");
	}

	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
		const std::string& channelName = *it;
		std::map<std::string, Channel*>::iterator chanIt = _channels.find(channelName);
		if (chanIt == _channels.end()) {
			sendError(fd, channelName, ERR_NOSUCHCHANNEL);
			continue;
		}

		Channel& channel = *(chanIt->second);
		std::vector<Client*>& members = channel.getMembers();
		Client* targetClient = NULL;
		for (std::vector<Client*>::iterator it2 = members.begin(); it2 != members.end(); ++it2) {
			if ((*it2)->getClient_fd() == fd) {
				targetClient = *it2;
				break;
			}
		}
		if (targetClient) {
			channel.removeMembers(targetClient);
			targetClient->removeChannel(channelName);
			std::cout << "Client " << fd << " removed from channel " << channelName << std::endl;
			std::string partMessage = ":" + targetClient->getNickName() + " PART " + channelName;
			if (!message.empty()) {
				partMessage += " :" + message;
			}
			partMessage += "\r\n";
			broadcastMessage(partMessage, NULL);
			removeEmptyChannels();
		} else {
			sendError(fd, channelName, ERR_NOTONCHANNEL);
		}
	}
}
