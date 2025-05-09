#include "../includes/Server.hpp"
#include <ctime>

bool	Server::_serverOnline = true;

static std::string getWordAfter(std::string str, std::string target) {
	size_t pos = str.find(target);  // Find the target word

	if (pos != std::string::npos) {
		size_t startPos = str.find_first_not_of(" \t\n\r", pos + target.length());  // Skip the target word and space
		if (startPos != std::string::npos) {
			size_t endPos = str.find_first_of(" \t\n\r", startPos);;  // Find the next space after the target word
			if (endPos == std::string::npos) {
				endPos = str.length();  // If no space is found, take the rest of the string
			}
			return str.substr(startPos, endPos - startPos);  // Extract the next word
		}
	}

	return "";  // Return an empty string if the target word is not found
}

Server::Server(int port, std::string password) : _port(port), _password(password), _serverName("Le_plus_beau_des_servers") {}

Server::~Server() {
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		delete it->second; // Libère chaque canal
_channels.clear(); //
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		delete it->second; // Libère chaque client
_clients.clear(); //
close(_server_fd);
}

void	Server::signalHandler(int sigNum) {
	(void)sigNum;
	_serverOnline = false;
}

//Getters

int	Server::getPort() const {
	return (_port);
}

int	Server::getServer_fd() const {
	return (_server_fd);
}

Channel*	Server::getChannel(std::string channelName) {
	std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
	if (it == _channels.end()) {
		return (NULL);
	}
	return it->second;
}

std::string	const&  Server::getPassword() const {
	return (_password);
}

std::string const&  Server::getServerName() const {
	return (_serverName);
}

//Setters
void	Server::setPort(int port) {
	this->_port = port;
}

void	Server::setServer_fd(int server_fd) {
	this->_server_fd = server_fd;
}

void	Server::setPassword(std::string const& password) {
	this->_password = password;
}

//Methodes

void	Server::serverInit() {
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd == -1){
		std::cerr << "Socket error." << std::endl;
		return ;
	}
	std::cout << "Socket Server: " << _server_fd << std::endl;//debug
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY; //pour que le resver réponde sur toutes ces interfaces résaeu (local ou internet)??
	server_addr.sin_port = htons(_port);
	int opt = 1;
	setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(_server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
		std::cerr << "Bind error." << std::endl;
		close(_server_fd);
		exit(EXIT_FAILURE);
	}
	std::cout << "Serveur prêt à écouter sur le port "  << _port << "..." << std::endl;
	if(listen(_server_fd, 10) == -1)//test avec 10 connexions simultanées, ajouter gestion erreur
	{
		// perror("bind");
		close(_server_fd);
		exit(EXIT_FAILURE);
	}
	std::cout << "Serveur en écoute..." << std::endl;
}

void	Server::setupServer() {
	struct pollfd server_pollfd;
	server_pollfd.fd = _server_fd;
	server_pollfd.events = POLLIN;
	_fds.push_back(server_pollfd);
}

void	Server::acceptClient() {
	// time_t start_time = time(NULL);  // Get the current time
	// time_t timeout_duration = 10;    // Timeout duration in seconds (10 seconds)
	// while (time(NULL) - start_time < timeout_duration) {
	// std::cout << "I m here" << std::endl;
	while (_serverOnline) {
		int poll_count = poll(_fds.data(), _fds.size(), -1);
		// std::cout << poll_count << std::endl;
		if (poll_count == -1) {
			// perror("poll");
			break ;
		}
		for (size_t i = 0; i < _fds.size(); ++i) {
			if (_fds[i].revents & POLLIN) {
				if (_fds[i].fd == _server_fd) {
					struct sockaddr_in client_addr;
					socklen_t client_len = sizeof(client_addr);
					int client_fd = accept(_server_fd, (struct sockaddr*)&client_addr, &client_len);
					if (client_fd == -1) {
						// perror("");
						continue ;
					}
					// std::string msg = "Enter PASS password\nNICK nickname\nUSER username 0 * :username\n";
					// send(client_fd, msg.c_str(), msg.size(), 0);
					struct pollfd client_pollfd;
					client_pollfd.fd = client_fd;
					client_pollfd.events = POLLIN;
					_fds.push_back(client_pollfd);
					_clients[client_fd] = new Client(client_fd);
					std::cout << "New client connected: " << client_fd << std::endl;
					// printClients();
				} else {
					handleClient(_fds[i].fd);
				}
			}
		}
	}
}

void	Server::handleClient(int client_fd) {
	char buffer[1024];
	ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
	if (bytes_read <= 0) {
		disconnectClient(client_fd);
	} else {
		buffer[bytes_read] = '\0';//!
		Client *client = _clients[client_fd];
		if (!client->getIsAuthentified())
		{
			// if (!client->getIsAuthentified() && client->getUserName().empty())
			// {
			// 	std::string msg = "Enter password: ";
			// 	send(client_fd, msg.c_str(), msg.size(), 0);
			// }
				std::string password = buffer;
				if (password.substr(0, 5) != "PASS " )
				{
					disconnectClient(client_fd);
					std::cout << client->getClient_fd() << ": Authentication failed\n";//! TODEBUG
					return ;
				}
				password = getWordAfter(password, "PASS");
				std::cout << password << std::endl;//! TODEBUG
				if (password == this->_password)
					{
						client->setIsAuthentified(true);
						std::cout << client->getClient_fd() << ": Authentication successful\n";//! TODEBUG
					}
					else
					{
						std::string msg = "Authentication failed2\n";
						// send(client_fd, msg.c_str(), msg.size(), 0);
						disconnectClient(client_fd);
					}
		} else if (client->getNickName().empty() || client->getUserName().empty()) {
				if (client->getNickName().empty())
				{
					std::string nickname = buffer;
					nickname = getWordAfter(nickname, "NICK");
					if (!userAlreadyRegistered(nickname))
					{
						client->setNickName(nickname);
						if (!client->getNickName().empty())
							std::cout << "nickname " << client->getNickName() << " nickname" << std::endl;
					}
					if (userAlreadyRegistered(nickname))
						sendError(client->getClient_fd(), nickname, ERR_NICKNAMEINUSE);
				}
				if (client->getUserName().empty())
				{
					std::string username = buffer;
					username = getWordAfter(username, "USER");
					client->setUserName(username);
					if (!client->getUserName().empty())
						std::cout << "username " << client->getUserName() << " username" << std::endl;
				}
				if (!client->getUserName().empty() && !client->getNickName().empty())
					sendWelcomeMessage(client->getClient_fd(), client->getNickName(), client->getUserName());
		} else {
			std::cout << "Received from client " << client->getUserName() << ": " << buffer;
			std::string pong = buffer;
			// Traiter les commandes du client ici
			std::string fragment(buffer);
			client->addMessageFragment(fragment);
			client->printQueue();
			while (client->hasCompleteMessage()) {
				std::string completeMessage = client->getNextMessage();
				if (!completeMessage.empty() && (completeMessage.back() == '\n' || completeMessage.back() == '\r'))
					completeMessage.pop_back();
				parseCommand(completeMessage, client_fd);
			}
		}
	}
}

void Server::sendWelcomeMessage(int clientSocket, const std::string& nickname, const std::string& user) {
	std::string welcome = "Welcome to the IRC Network, " + nickname + "!" + user + "@" + _serverName;
	std::string version = "0.42";
	std::string userModes = "or";
	std::string channelModes = "itkol";
	std::string yourHost = "Your host is " + _serverName + ", running version " + version;
	std::string serverCreated = "This server was created Mon Dec 23 2024 at 08:14:10 EST";
	std::string serverInfo = _serverName + " " + version + " " + userModes + " " + channelModes;

	// Send responses to client
	sendToClient(clientSocket, sendResponse("001", nickname+ " :", welcome));// 001: Welcome message
	sendToClient(clientSocket, sendResponse("002", nickname+ " :", yourHost));// 002: Host version info
	sendToClient(clientSocket, sendResponse("003", nickname+ " :", serverCreated));// 003: Server created time
	sendToClient(clientSocket, sendResponse("004", nickname+ " :", serverInfo));// 004: Server info
}

std::string Server::sendResponse(const std::string& command, const std::string& nickname, const std::string& message) {
	return ":" + _serverName + " " + command + " " + nickname + " " + message + "\r\n";
}

void	Server::sendToClient(int clientSocket, const std::string& message) {
	std::cout << message;//DEBUG
	send(clientSocket, message.c_str(), message.length(), 0);
}

void Server::broadcastMessage(const std::string& message, const std::vector<std::string>* channelNames) {
	std::set<int> targetClients; // Un set pour éviter les duplications de clients (FDs)

	if (channelNames && !channelNames->empty()) {
		// Si une liste de channels est fournie, ajouter les membres de ces channels
		for (std::vector<std::string>::const_iterator chIt = channelNames->begin(); chIt != channelNames->end(); ++chIt) {
			std::string channelName = *chIt;

			// Chercher le channel par son nom dans la map
			std::map<std::string, Channel*>::const_iterator channelIt = _channels.find(channelName);
			if (channelIt != _channels.end()) {
				// Channel trouvé, on ajoute les membres à la liste des destinataires
				const std::vector<Client*>& members = channelIt->second->getMembers();
				for (std::vector<Client*>::const_iterator memIt = members.begin(); memIt != members.end(); ++memIt) {
					targetClients.insert((*memIt)->getClient_fd());
				}
			}
		}
		std::cout << "Broadcasting message to specified channels only." << std::endl;
	} else {
		// Si aucun channel n'est spécifié, envoyer à tous les clients connectés
		for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
			targetClients.insert(it->first);  // On insère l'FD de chaque client
		}
		std::cout << "Broadcasting message to all connected clients." << std::endl;
	}

	// Envoyer le message à tous les clients dont les FD ont été collectés
	for (std::set<int>::iterator fdIt = targetClients.begin(); fdIt != targetClients.end(); ++fdIt) {
		int clientFd = *fdIt;
		if (send(clientFd, message.c_str(), message.length(), 0) == -1) {
			// Afficher un message d'erreur en cas d'échec
			std::cerr << "Error: Unable to send message to client FD " << clientFd << std::endl;
		}
	}

	// Afficher dans la console que le message a été envoyé
	std::cout << "Broadcast message sent: " << message << std::endl;
}

void	Server::disconnectClient(int fd) {
	close(fd);
	for (size_t i = 0; i < _fds.size(); ++i) {
		if (_fds[i].fd == fd) {
			_fds.erase(_fds.begin() + i);
			break;
		}
	}
	std::map<std::string, Channel *>::iterator it = _channels.begin();
	while (it != _channels.end())
	{
	it->second->removeMembers(_clients[fd]);
	it++;
	}
	
	_clients.erase(fd);
	std::cout << "Client disconnected: " << fd << std::endl;
}

//! À verifier si nessesaire
void Server::sendError(int fd, const std::string& target, const std::string& message) {
	std::ostringstream response;//equivalent a l'operateur + pour concatener les chaine de caractere (ne fait de copie et convertis automatiquement en string)
	response << ":" << this->getServerName() << " " << target << " :" << message << "\r\n"; //peut etre enlever le nom du server
	send(fd, response.str().c_str(), response.str().length(), 0);
}

bool	Server::userAlreadyRegistered(std::string nickname) {
	if (nickname.empty())
		return false;
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second->getNickName() == nickname)
			return true;
	}
	return false;
}

Client*	Server::isClient(int fd) {
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if (it == _clients.end()) {
		sendError(fd, "", ERR_NOTREGISTERED);
		return (NULL);
	}
	return it->second;
}

void Server::removeEmptyChannels() {
	// Parcourir tous les canaux
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end();) {
		Channel* channel = it->second;

		// Si le canal est vide (aucun membre), on le détruit
		if (channel->getMembers().empty()) {
			std::cout << "Le canal " << it->first << " est vide, suppression." << std::endl;

			// On supprime le canal du serveur
			delete channel;
			_channels.erase(it++);
		} else {
			++it;
		}
	}
}

Client* Server::getClientByNickname(const std::string& nickname) {
    // Parcourir la map des clients pour trouver celui qui correspond au nickname
    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it) {
        Client* client = it->second; // Récupère le pointeur vers le client
        if (client->getNickName() == nickname) {
            return client; // Retourne le client si le nickname correspond
        }
    }
    return NULL; // Aucun client trouvé avec ce nickname
}

void	Server::printClients() {
	std::cout << "Liste des clients connectés :\n";
	// Parcourir chaque client dans la map
	std::map<int,Client*>::iterator it = _clients.begin();
	int i = 0;
		while (it != _clients.end() && _clients.size() > 0) {
			std::cout << "Client " << i + 1 << " :\n";
			std::cout << "FD du client : " << it->second->getClient_fd() << std::endl;  // Utilisez it->second pour accéder à Client*
			++it;  // Déplacement de l'itérateur vers l'élément suivant
			++i;   // Incrémentation de l'index
	}
		std::cout << "-------------------------------\n";
}
