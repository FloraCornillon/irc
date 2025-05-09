#include "../includes/Channel.hpp"
#include "../includes/utils.hpp"

Channel::Channel() {}
Channel::~Channel() {}

//Checké si compatible c++98!!!Je ne crois pas
// Channel::Channel(const std::string &name): _name(name) {
// 	_mode['i'] = false; //* false=no invit needed | true=only invitation
// 	_mode['t'] = false; //*false=all change | true=just operator change topic
// 	_mode['k'] = false; //* false=no password | true=password
// 	_mode['l'] = false; //
// 	// _mode['o'] = /MODE #channel -o nick
// }

Channel::Channel(Server &server, const std::string &name) : _server(&server), _name(name) {
	_mode.insert(std::make_pair('i', false));
	_mode.insert(std::make_pair('t', false));
	_mode.insert(std::make_pair('k', false));
	_mode.insert(std::make_pair('l', false));
}


//Getters
std::string const&	Channel::getName() const {
	return (_name);
}

std::string const&	Channel::getTopic() const {
	return (_topic);
}

std::string const&	Channel::getPassword() const {
	return (_password);
}

size_t	Channel::getMaxUsers() const {
	return (_maxUsers);
}

std::vector<Client*>& Channel::getMembers() {
	return (_members);
}

//Setters

void	Channel::setName(std::string const& name) {
	this->_name = name;
}

void	Channel::setTopic(std::string const& topic) {
	this->_topic = topic;
}

void	Channel::setPassword(std::string const& password) {
	this->_password = password;
}

void	Channel::setMaxUsers(size_t maxUsers) {
	this->_maxUsers = maxUsers;
}

void Channel::setMode(char mode, bool value, int fd) {
	// Vérifie si le mode existe déjà
	std::map<char, bool>::iterator it = _mode.find(mode);

	// Si le mode existe déjà, met à jour sa valeur
	if (it != _mode.end()) {
		it->second = value;
		listMembers();
	} else {
		// Si le mode n'existe pas encore, l'ajoute avec la valeur spécifiée
		_server->sendError(fd, "", ERR_UNKNOWNMODE);
		std::cout << "errorcaca\n";
	}
}

//Methodes
void	Channel::addMembers(Client* client) {
	if (isMode('i') && !isInvited(client))
		return ;
	if (std::find(_members.begin(), _members.end(), client) == _members.end()) {
		_members.push_back(client);
	}
}

void	Channel::removeMembers(Client* client) {
	removeOperator(client);
	std::vector<Client*>::iterator it = std::find(_members.begin(), _members.end(), client);
	if (it == _members.end())
		return ;
	_members.erase(it);
}

void	Channel::addOperator(Client* client) {
	std::vector<Client*>::iterator it = std::find(_operateurs.begin(), _operateurs.end(), client);
	if (it != _operateurs.end())
		return ;
	_operateurs.push_back(client);
}

void	Channel::removeOperator(Client* client) {
	std::vector<Client*>::iterator it = std::find(_operateurs.begin(), _operateurs.end(), client);
	if (it == _operateurs.end())
		return ;
	_operateurs.erase(it);
}

std::string Channel::getMembersAsString() const {
	std::string result;
	for (std::vector<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
		Client* member = *it;

		// Check if the member is an operator
		if (std::find(_operateurs.begin(), _operateurs.end(), member) != _operateurs.end()) {
			result += "@"; // Prefix for channel operators
		}
		result += member->getNickName() + " ";
	}
	// Remove trailing space if necessary
	if (!result.empty() && result[result.size() - 1] == ' ') {
		result.erase(result.size() - 1);
	}
	return result;
}

Client*	Channel::isMember(int fd) {
	for (std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); ++it) {
	if ((*it)->getClient_fd() == fd)
		return *it;
	}
	return NULL;
}

Client*	Channel::isOperator(int fd) {
	for (std::vector<Client*>::iterator it = _operateurs.begin(); it != _operateurs.end(); ++it) {
	if ((*it)->getClient_fd() == fd)
		return *it;
	}
	return NULL;
}

bool Channel::isMode(char mode) const {
	std::map<char, bool>::const_iterator it = _mode.find(mode);
	if (it != _mode.end()) {
		return it->second;  // Retourne true si le mode est actif, sinon false
	}
	return false;  // Si le mode n'est pas trouvé, il n'est pas actif
}

//DEBUG
void Channel::listMembers() const {
	std::cout << "Members in channel " << _name << ":" << std::endl;
	for (std::vector<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
		std::cout << "- " << (*it)->getNickName() << std::endl;
	}
	std::cout << "Operators in channel " << _name << ":" << std::endl;
	for (std::vector<Client*>::const_iterator it = _operateurs.begin(); it != _operateurs.end(); ++it) {
		std::cout << "- " << (*it)->getNickName() << std::endl;
	}
		std::cout << "Modes in channel " << _name << ":" << std::endl;
	for (std::map<char, bool>::const_iterator it = _mode.begin(); it != _mode.end(); ++it) {
		std::cout <<it->first << "- " << it->second << std::endl;
	}
	std::cout << "Topic of the channel " << getTopic() << std::endl;
	std::cout << "Channel user limit " << getMaxUsers() << std::endl;

	std::cout << "list of invited nickNames " << _name << ":" << std::endl;
	for (std::vector<Client*>::const_iterator it = _invited.begin(); it != _invited.end(); ++it) {
		std::cout << "- " << (*it)->getNickName() << std::endl;
	}
}

// Ajouter un utilisateur à la liste d'invités
void Channel::inviteUser(Client* client) {
	if (std::find(_invited.begin(), _invited.end(), client) == _invited.end()) {
		_invited.push_back(client);
	}
}

// Vérifier si un utilisateur est invité
bool Channel::isInvited(Client* client) const {
	return std::find(_invited.begin(), _invited.end(), client) != _invited.end();
}