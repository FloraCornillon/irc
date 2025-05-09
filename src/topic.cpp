#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"
#include "../includes/utils.hpp"


void	Server::handle_topic_command(const std::string &command, int fd) {
	std::istringstream iss(command);
	std::string cmd, channelName, topic;
	iss >> cmd >> channelName; // Extraire "TOPIC" et le nom du channel
	
	// Vérifier si le channel existe
	if (_channels.find(channelName) == _channels.end()) {
		sendError(fd, channelName, ERR_NOSUCHCHANNEL);
		return;
	}
	
	Channel *channel = _channels[channelName];

	// Vérifier si le client est membre du channel
	if (channel->isMember(fd) == NULL) {
		sendError(fd, channelName, ERR_NOTONCHANNEL);
		return;
	}
	
	// Extraire le sujet s'il existe
	std::getline(iss, topic);
	if (topic.empty()) {
		// Pas de sujet fourni, envoyer le sujet actuel
		if (channel->getTopic().empty()) {
			sendToClient(fd, channelName + " :" + ERR_NOTEXTTOSEND);
		} else {
			sendToClient(fd, channelName + " :" + channel->getTopic());
		}
	} else {
		// Supprimer le caractère ':' au début du sujet
		if (topic[0] == ':') {
			topic.erase(0, 1);
		}

		// Mettre à jour le sujet du channel
		channel->setTopic(topic);
		sendToClient(fd, "TOPIC " + channelName + " :" + topic);
		//BROADCAST a faire??
	}
}