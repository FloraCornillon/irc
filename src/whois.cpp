#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"
#include "../includes/utils.hpp"


void Server::whois_command(const std::string& targetNick, int fd) {
    // Vérifier si le client est connecté
    Client* client = _clients[fd];
    if (!client) {
        sendError(fd, "", ERR_USERNOTFOUND);
        return;
    }

    // Chercher l'utilisateur cible
    Client* targetClient = getClientByNickname(targetNick); //a ecrire!!!
    if (!targetClient) {
        sendError(fd, targetNick, "ERR_NOSUCHNICK: No such nickname");
        return;
    }

    // Informations de base
    std::string nickname = targetClient->getNickName();
    std::string username = targetClient->getUserName();

    // Construction de la réponse
    std::string response;
    response = ":server WHOIS " + nickname + " " + username + " " + "\r\n";
    
    // Ajouter des informations supplémentaires comme les canaux et si c'est un opérateur
    std::vector<std::string> channels = targetClient->getChannels();
    for (size_t i = 0; i < channels.size(); ++i) {
        response += " " + channels[i];
    }

    if (targetClient->getIsOperator()) {
        response += " :is an IRC operator";
    }

    // Envoyer la réponse au client qui a fait la demande
    sendToClient(fd, response);

    // Afficher la réponse dans la console pour vérification
    std::cout << "WHOIS response for " << targetNick << ": " << response << std::endl;
}