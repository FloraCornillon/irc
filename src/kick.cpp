#include "../includes/Server.hpp"

void Server::kick_command(std::string command, int fd) {
	std::istringstream stream(command);
	std::string commandType, channelsPart, usersPart, reason;

	stream >> commandType; // "KICK"
	stream >> channelsPart; // Channels séparés par des virgules
	stream >> usersPart; // Utilisateurs séparés par des virgules

	if (stream) {
		std::getline(stream, reason);
		reason = trim(reason, " \t\r\n:");
	}

	if (channelsPart.empty() || usersPart.empty()) {
		sendError(fd, "", ERR_NEEDMOREPARAMS); // Pas assez de paramètres
		return;
	}

	// Séparer les channels et les utilisateurs
	std::vector<std::string> channels = split(channelsPart, ',');
	std::vector<std::string> users = split(usersPart, ',');

	// Parcourir chaque canal
	for (size_t i = 0; i < channels.size(); ++i) {
		std::string channelName = trim(channels[i], " \t\r\n");

		// Vérifier si le channel existe
		std::map<std::string, Channel*>::iterator chanIt = _channels.find(channelName);
		if (chanIt == _channels.end()) {
			sendError(fd, channelName, ERR_NOSUCHCHANNEL);
			continue;
		}

		Channel& channel = *(chanIt->second);

		if (channel.isOperator(fd) == NULL) {
        sendError(fd, channelName, ERR_CHANOPRIVSNEEDED);
        return ;
    }

		for (size_t j = 0; j < users.size(); ++j) {
			std::string targetNick = trim(users[j], " \t\r\n");

			// Trouver l'utilisateur cible
			std::vector<Client*>& members = channel.getMembers();
			Client* targetClient = NULL;

			for (std::vector<Client*>::iterator it = members.begin(); it != members.end(); ++it) {
				if ((*it)->getNickName() == targetNick) {
					targetClient = *it;
					break;
				}
			}
			if (!targetClient) {
				sendError(fd, channelName, ERR_USERNOTINCHANNEL);
				continue;
			}

			std::string banMessage = targetNick + " :" + ERR_YOUREBANNEDCREEP + "\r\n";
			sendToClient(targetClient->getClient_fd(), banMessage);

			// Retirer le client du canal
			channel.removeMembers(targetClient);
			targetClient->removeChannel(channelName);

			// Construire et diffuser le message KICK
			std::string kickMessage = ":" + _clients[fd]->getNickName() + " KICK " + channelName + " " + targetNick;
			if (!reason.empty()) {
				kickMessage += " :" + reason;
			}
			kickMessage += "\r\n";

			std::vector<std::string> channelNames;
			channelNames.push_back(channelName); // Liste des channels à spécifier
			broadcastMessage(kickMessage, &channelNames);

			std::cout << "User " << targetNick << " kicked from channel " << channelName << " by " << _clients[fd]->getNickName() << std::endl;
		}
		removeEmptyChannels();
	}
}