#include "../includes/Server.hpp"


void Server::privmsg_command(const std::string& command, int fd) {
    // Obtenir le client qui envoie le message
    Client* sender = isClient(fd);
    if (!sender) return;

    // Parsing de la commande avec std::istringstream
    std::istringstream stream(command);
    std::string cmd, target, temp;
    std::string message;

    stream >> cmd;

    // Extraire le destinataire (target)
    stream >> target;
    if (target.empty()) {
        sendError(fd, "", ERR_NORECIPIENT);
        return;
    }

    // Lire tout ce qui reste dans le flux pour former le message
    if (stream.peek() == ' ') stream.get(); // Ignorer l'espace après le target
    while (std::getline(stream, temp, '\n')) {
        message += temp + "\n"; // Ajouter chaque ligne lue
    }
    if (!message.empty() && message[message.size() - 1] == '\n') {
        message.erase(message.size() - 1); // Retirer le dernier saut de ligne
    }

    if (message.empty()) {
        sendError(fd, target, ERR_NOTEXTTOSEND);
        return;
    }

    // Gestion des cas : canal ou utilisateur
    if (target[0] == '#') {
        // Cas 1: Message à un canal
        std::map<std::string, Channel*>::iterator it = _channels.find(target);
        if (it == _channels.end()) {
            sendError(fd, target, ERR_NOSUCHCHANNEL);
            return;
        }

        Channel* channel = it->second;

        // Vérifier que l'expéditeur est membre du canal (si mode +n activé)
        if (!channel->isMember(fd)) {
            sendError(fd, target, ERR_CANNOTSENDTOCHAN);
            return;
        }

        // Envoyer le message à tous les membres sauf l'expéditeur
        std::vector<Client*>::iterator clientIt;
        for (clientIt = channel->getMembers().begin(); clientIt != channel->getMembers().end(); ++clientIt) {
            if ((*clientIt)->getClient_fd() != fd) {
                sendToClient((*clientIt)->getClient_fd(), 
                    ":" + sender->getNickName() + " PRIVMSG " + target + " " + message + "\r\n");
            }
        }
    } else {
        // Cas 2: Message à un utilisateur
        for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
            if (it->second->getNickName() == target) {
                sendToClient(it->second->getClient_fd(), 
                    ":" + sender->getNickName() + " PRIVMSG " + target + " " + message + "\r\n");
                return;
            }
        }
        sendError(fd, target, ERR_NOSUCHNICK); // Si utilisateur non trouvé
    }
}