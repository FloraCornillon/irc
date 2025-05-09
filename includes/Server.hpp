#pragma once

#define RED "\001\e[31m\002"
#define GRN "\001\e[32m\002"
#define YEL "\001\e[33m\002"
#define BLU "\001\e[34m\002"
#define PNK "\001\e[35m\002"
#define RST "\001\e[0m\022"

#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <unistd.h> //pour close()
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <poll.h>
#include <set>

class Channel;
class Client;

class Server {
	private:
		int								_port; //le port sur lequel le serveur écoute
		int 							_server_fd; //le FD du serveur créé par socket()
		std::map<int, Client*>			_clients; //les clients connectés au serveur
		std::map<std::string, Channel*>	_channels; //map por stocker les channel, changer pour pointeur si besoin d'acès spécifique...
		std::vector<struct pollfd>		_fds;
		std::string						_password;
		const std::string				_serverName;
		static bool						_serverOnline;


	public:
		Server();
		Server(int port, std::string password); //Constructeur
		~Server();

	//Getters
		int							getPort() const;
		int							getServer_fd() const;
		std::string	const&			getPassword() const;
		std::string const&			getServerName() const;

	//Setters
		void						setPort(int port);
		void						setServer_fd(int server_fd);
		void						setPassword(std::string const& password);
	
	//Methodes
		static void					signalHandler(int sigNum);
		void						serverInit();
		void						setupServer(); //TODO config serveur (socket, bind, listen)
		void						acceptClient(); //TODO accepte un client et cree un objet Client
		void						handleClient(int client_fd); //TODO gere les messages d'un client
		void						parseCommand(const std::string &command, int fd);
		void						broadcastMessage(const std::string& message, const std::vector<std::string>* channelNames); //TODO envoie un message à tous les clients
		void						disconnectClient(int fd); //TODO déconnecte un client
		void						sendToClient(int clientSocket, const std::string& message);
		std::string					sendResponse(const std::string& command, const std::string& nickname, const std::string& message);
		void						sendError(int fd, const std::string& target, const std::string& message);
		void						sendWelcomeMessage(int clientSocket, const std::string& nickname, const std::string& user);
		Client*						isClient(int fd);
		Channel*					getChannel(std::string chanelName);
	
	//commande (à déplacer ailleurs si pertinent)
		void						join_command(const std::string &command, int fd); //creation ou rejoindre channel, toujours fait par server
		void						part_command(const std::string &command, int fd);//quitter un channel ou le server
		void						privmsg_command(const std::string &command, int fd);//envoyer un message privé ou un message a tt les utilisateur d'un channel
		void						handle_topic_command(const std::string &command, int fd); //La commande topic va appeler les fonctions de la classe channel 
		void						invite_command(const std::string &command, int fd);
		void						nick_command(const std::string& command, int fd);
		void						removeEmptyChannels();
		bool						userAlreadyRegistered(std::string username);
		void						kick_command(std::string command, int fd);
		void						whois_command(const std::string& targetNick, int fd);
		Client*						getClientByNickname(const std::string& nickname);
		void						send_names_to_client(int fd, const std::string& channel_name, const std::vector<Client*>& members);

//debug
		void							printClients();
};