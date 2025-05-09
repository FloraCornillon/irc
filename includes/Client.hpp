#pragma once

#define RED "\001\e[31m\002"
#define GRN "\001\e[32m\002"
#define YEL "\001\e[33m\002"
#define BLU "\001\e[34m\002"
#define PNK "\001\e[35m\002"
#define RST "\001\e[0m\022"

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unistd.h> //pour close()

class Client {
	private:
		// bool	_superuser;
		// std::string _userName;
		// std::string _nickName;
		int 					_client_fd; //le FD du client créé par accept()
		bool					_isAuthentified;
		std::string				_currentChannel;
		bool					_isOperator;
		std::string				_nickName;
		std::string				_userName;
		std::queue<std::string>	_messages;
		std::vector<std::string> _channels;

	public:
		Client();
		Client(int fd); //Constructeur
		~Client();

		//getters
		int 				getClient_fd() const;
		std::string const&	getNickName() const;
		std::string const&	getUserName() const;
		std::string const&	getCurrentChannel() const;
		bool 				getIsAuthentified() const;
		bool 				getIsOperator() const;
		std::vector<std::string> const& getChannels() const;

		//Setters
		void 				setClient_fd(int client_fd);
		void 				setNickName(std::string const& nickName);
		void 				setUserName(std::string const& userName);
		void 				setCurrentChannel(std::string const& currentChannel);
		void 				setIsAuthentified(bool isAuthentified);
		void 				setIsOperator(bool isOperator);

		//Methodes
		void				authentify(std::string password); //TODO
		void				joinChannel(std::string channel); //TODO pour rejoindre un channel
		void				leaveChannel(); //TODO pour quitter un channel
		void				sendMessage(std::string message); //TODO pour envoyer un message
		std::string			receiveMessage(); //TODO lit le message entrant du client
		void				addMessageFragment(const std::string& fragment); // Ajouter un fragment de message
        bool				hasCompleteMessage() const; // Vérifie si une commande complète est disponible
        std::string			getNextMessage();
		void				updateChannelList(const std::string& channel_name);
		void				removeChannel(const std::string& channelName);

		//DEBUG
		void				printQueue() const;
};