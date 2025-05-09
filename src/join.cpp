#include "../includes/Server.hpp"
#include "../includes/utils.hpp"

void Server::send_names_to_client(int fd, const std::string& channel_name, const std::vector<Client*>& members) {
    std::stringstream names_message;
    names_message << ":irc.server.com 353 " << fd << " = " << channel_name << " :";

    // Ajouter les utilisateurs au message
    for (size_t i = 0; i < members.size(); ++i) {
        names_message << members[i]->getNickName();
        if (i != members.size() - 1) {
            names_message << " ";  // Ajouter un espace entre les noms
        }
    }

    // Terminer le message avec le code 366 (ENDOFNAMES)
    names_message << "\r\n";
    send(fd, names_message.str().c_str(), names_message.str().length(), 0);

    // Message de fin
    std::stringstream end_message;
    end_message << ":irc.server.com 366 " << fd << " " << channel_name << " :End of /NAMES list.\r\n";
    send(fd, end_message.str().c_str(), end_message.str().length(), 0);
}


void Server::join_command(const std::string &command, int fd) {
	Client *client = isClient(fd);
	if (!client)
		return;

	size_t pos = command.find(' ');
	if (pos == std::string::npos) {
		sendError(fd, "", ERR_NEEDMOREPARAMS);
		return;
	}

	std::istringstream params(command.substr(pos + 1));
	std::string channel_name, password;

	params >> channel_name;
	params >> password; // Le mot de passe est optionnel

	std::cout << "Channel name: " << channel_name << std::endl;
	std::vector<std::string> channels = split(channel_name, ',');
	std::vector<std::string> passwords = split(password, ',');
	for (size_t i = 0; i < channels.size(); ++i) {
		std::string channel_name = channels[i];
		std::string password = (i < passwords.size()) ? passwords[i] : "";

		if (channel_name.empty() || (channel_name[0] != '#' && channel_name[0] != '&')) {
			sendError(fd, channel_name, ERR_NOSUCHCHANNEL);
			continue;
		}

	if (channel_name.empty() || (channel_name[0] != '#' && channel_name[0] != '&')) {
		sendError(fd, channel_name, ERR_NOSUCHCHANNEL);
		return;
	}

	// Vérification ou création du canal
	Channel* channel = NULL;
	std::map<std::string, Channel*>::iterator it = _channels.find(channel_name);

	if (it == _channels.end()) {
		channel = new Channel(*this, channel_name);
		_channels[channel_name] = channel;
		channel->addOperator(client);
		std::cout << "Channel: " << channel_name << " created" << std::endl;
	} else {
		channel = it->second;
	}

	// Vérifications des modes
	if (channel->isMode('i') && !channel->isInvited(client)) {
		sendError(fd, channel_name, ERR_INVITEONLYCHAN);
		return;
	}

	if (channel->isMode('k') && channel->getPassword() != password) {
		sendError(fd, channel_name, ERR_BADCHANNELKEY);
		return;
	}

	if (channel->isMode('l') && channel->getMembers().size() >= static_cast<size_t>(channel->getMaxUsers())) {
		sendError(fd, channel_name, ERR_CHANNELISFULL);
		return;
	}
	if (channel->isMember(fd)) {
        sendError(fd, channel_name, ERR_ALREADYREGISTERED);
        return;
    }
	channel->addMembers(client);
	client->joinChannel(channel_name);
	client->updateChannelList(channel_name);
	//DEBUG!!!!!!
	// std::cout << "Client " << fd << " channels list after joining: ";
    //     std::vector<std::string> clientChannels = client->getChannels();
    //     for (size_t i = 0; i < clientChannels.size(); ++i) {
    //         std::cout << clientChannels[i] << " ";
	// 	}
    //     std::cout << std::endl;
	//fin DEBUG

	std::cout << "Client " << fd << " joined channel " << channel_name << std::endl;
	std::cout << "Current members of " << channel_name << ":" << std::endl;
	channel->listMembers();

	std::string joinedChannelMessage = ":" + client->getNickName() + "!"  + client->getUserName() + "@" + _serverName + " JOIN :" + channel_name + "\r\n";
	sendToClient(fd, joinedChannelMessage);
	// std::vector<Client*> members = channel->getMembers();
    //     for (size_t i = 0; i < members.size(); ++i) {
    //         send_names_to_client(members[i]->getClient_fd(), channel_name, members);
    //     }
	}
	std::vector<std::string> singleChannel;
    singleChannel.push_back(channel_name);

	std::string broadcastMessageContent = ":" + client->getNickName() + "!"  + client->getUserName() + "@" + _serverName + " JOIN :" + channel_name + "\r\n";
    broadcastMessageContent += "\r\n";  // Format pour IRC

    broadcastMessage(broadcastMessageContent, &singleChannel);
}

