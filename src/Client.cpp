#include "../includes/Client.hpp"

//Constructors / Destructor
Client::Client() {}

Client::Client(int fd) : _client_fd(fd), _isAuthentified(false), _isOperator(false) {}

Client::~Client() {
	close(_client_fd);
}

//Getters
int	Client::getClient_fd() const {
	return (_client_fd);
}

std::string const&	Client::getNickName() const {
	return (_nickName);
}

std::string const&	Client::getUserName() const {
	return (_userName);
}

std::string const&	Client::getCurrentChannel() const {
	return (_currentChannel);
}

void Client::updateChannelList(const std::string& channel_name) { 
    _channels.push_back(channel_name);
} //ajouter check pour pas entrer 2 x meme channel

std::vector<std::string> const& Client::getChannels() const {
    return _channels;
}

bool	Client::getIsAuthentified() const {
	return (_isAuthentified);
}

bool	Client::getIsOperator() const {
	return (_isOperator);
}

//Setters
void	Client::setClient_fd(int client_fd) {
	this->_client_fd = client_fd;
}

void	Client::setNickName(std::string const& nickName) {
	this->_nickName = nickName;
	}

void	Client::setUserName(std::string const& userName) {
	this->_userName = userName;
}

void	Client::setCurrentChannel(std::string const& currentChannel) {
	this->_currentChannel = currentChannel;
}

void	Client::setIsAuthentified(bool isAuthentified) {
	this->_isAuthentified = isAuthentified;
}

void	Client::setIsOperator(bool isOperator) {
	this->_isOperator = isOperator;
}

//Methodes
// void	Client::authentify(std::string password) {
// if (password == "correct_password") { //! Remplacer par la logique réelle
// 	_isAuthentified = true;
// std::cout << "Client authentifié" << std::endl;
// } else {
// 	std::cout << "Mot de passe incorrect" << std::endl;
// }
// } //TODO

void	Client::joinChannel(std::string channel) {
	this->_currentChannel = channel;
std::cout << "Client a rejoint le canal: " << channel << std::endl;
}

void	Client::leaveChannel() {
	std::cout << "Client a quitté le canal: " << _currentChannel << std::endl;
this->_currentChannel = "";
}

void	Client::sendMessage(std::string message) {
	this->_messages.push(message);
	std::cout << "Message envoyé: " << message << std::endl;
}

std::string	Client::receiveMessage() {
	if (_messages.empty()) {
		return ("");
	}
	std::string message = _messages.front();
	_messages.pop();
	std::cout << "Message reçu: " << message << std::endl;
	return (message);
}

void Client::addMessageFragment(const std::string& fragment) {
    _messages.push(fragment);
}

bool Client::hasCompleteMessage() const {
    if (_messages.empty())
        return false;
    // Vérifier si un message complet (avec '\n') est présent
    std::queue<std::string> temp = _messages;
    while (!temp.empty()) {
        if (temp.front().find('\n') != std::string::npos)
            return true;
        temp.pop();
    }
    return false;
}

std::string Client::getNextMessage() {
    std::string fullMessage;
    while (!_messages.empty()) {
        fullMessage += _messages.front();
        _messages.pop();
        if (fullMessage.find('\n') != std::string::npos)
            break;
    }
    return fullMessage;
}

void Client::removeChannel(const std::string& channelName) {
    std::vector<std::string>::iterator it = std::find(_channels.begin(), _channels.end(), channelName);
    if (it != _channels.end()) {
        _channels.erase(it);
    }
}

// void Client::removeChannel(const std::string& channelName) {
//     // Parcourir la liste des canaux du client
//     for (size_t i = 0; i < _channels.size(); ++i) {
//         if (_channels[i] == channelName) {
//             // Supprimer le canal de la liste en décalant les éléments
//             for (size_t j = i; j < _channels.size() - 1; ++j) {
//                 _channels[j] = _channels[j + 1];
//             }
//             _channels.pop_back();  // Retirer le dernier élément
//             return;  // Canal trouvé et supprimé, on peut sortir de la méthode
//         }
//     }
// }


//DEUG

void Client::printQueue() const {
    // Créer une copie temporaire de la file d'attente
    std::queue<std::string> tempQueue = _messages;

    // Afficher le contenu de la file
    std::cout << "Message queue for client " << _client_fd << ":" << std::endl;
    while (!tempQueue.empty()) {
        std::cout << tempQueue.front() << std::endl;
        tempQueue.pop();
    }
}
