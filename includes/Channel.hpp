#pragma once

#define RED "\001\e[31m\002"
#define GRN "\001\e[32m\002"
#define YEL "\001\e[33m\002"
#define BLU "\001\e[34m\002"
#define PNK "\001\e[35m\002"
#define RST "\001\e[0m\022"

#include "Client.hpp"
#include "Server.hpp"


#include <string>
#include <vector>
#include <algorithm>
#include <map>

class Client;
class Server;

class Channel {
	private:
		Server						*_server;
		std::string					_name;
		std::string					_topic; //le sujet du channel
		std::vector<Client*>		_members; //les membres du channel
		std::vector<Client*>		_operateurs; //les operateurs du channel
		std::map<char, bool>		_mode;
		std::string					_password;
		size_t						_maxUsers;
		std::vector<Client*> 		_invited; // Liste des utilisateurs invités

	public:
		Channel(); //constructeur
		Channel(Server &server, const std::string &name);
		~Channel();
		// void	kickUser(std::string userToKick, std::string reasons);
		// void	inviteUser(std::string userToInvite);
		// void	changeTopic(std::string newTopic);

	//Getters
		std::string const&		getName() const;
		std::string const&		getTopic() const;
		std::string const&		getPassword() const;
		size_t					getMaxUsers() const;
		std::vector<Client*>&	getMembers();
		std::string				getMembersAsString() const;

	//Setters
		void					setName(std::string const& name);
		void					setTopic(std::string const& topic);
		void					setPassword(std::string const& password);
		void					setMaxUsers(size_t maxUsers);
		void					setMode(char mode, bool value, int fd);

	//Methodes
		void					addMembers(Client* client);
		void					removeMembers(Client* client);
		void					addOperator(Client* client);
		void					removeOperator(Client* client);
		Client*					isMember(int fd);
		Client*					isOperator(int fd);
		bool					isMode(char mode) const;
		void 					inviteUser(Client* client); // Ajouter un utilisateur à la liste d'invités
		bool 					isInvited(Client* client) const; // Vérifier si un utilisateur est invité

		//command
		void					mode_command(std::istringstream &stream, int fd);

		//DEBUG
		void					listMembers() const;
};