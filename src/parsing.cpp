#include "../includes/Server.hpp"

std::string cleanString(const std::string &input) {
	std::string result;
	for (size_t i = 0; i < input.size(); ++i) {
		if (std::isprint(input[i]) || std::isspace(input[i])) {
			result += input[i];
		}
	}
	return result;
}


void Server::parseCommand(const std::string &command, int fd) {
	std::string cleanedCommand = cleanString(command);
	// std::cout << "Command debug: ";///DEBUG
	// for (size_t i = 0; i < cleanedCommand.size(); ++i) {
	//     std::cout << "[" << cleanedCommand[i] << "]";
	// }
	// std::cout << std::endl;

	std::istringstream stream(cleanedCommand);
	std::cout << cleanedCommand << std::endl;
	std::string commandType;
	std::string channelName;
	stream >> commandType;
	if (commandType == "/join" || commandType == "JOIN")
		return (join_command(cleanedCommand, fd));
	if (commandType == "/nick" || commandType == "NICK")
		return (nick_command(cleanedCommand, fd));
	if (commandType == "/part" || commandType == "PART")
		return (part_command(cleanedCommand, fd));
	if (commandType == "/privmsg" || commandType == "PRIVMSG")
		return (privmsg_command(cleanedCommand, fd));
	stream >> channelName;
	if ((commandType == "/topic" || commandType == "TOPIC") && getChannel(channelName) && !getChannel(channelName)->isMode('t'))
		return (handle_topic_command(cleanedCommand, fd));
	if ((commandType == "/topic" || commandType == "TOPIC") && getChannel(channelName) && getChannel(channelName)->isMode('t') && getChannel(channelName)->isOperator(fd))
		return (handle_topic_command(cleanedCommand, fd));
	if ((commandType == "/topic" || commandType == "TOPIC") && getChannel(channelName) && getChannel(channelName)->isMode('t') && !getChannel(channelName)->isOperator(fd))
		return (sendError(fd, _clients[fd]->getNickName(), ERR_CHANOPRIVSNEEDED));
	if (commandType == "/kick" || commandType == "KICK")
		return (kick_command(cleanedCommand, fd));
	if (commandType == "/invite" || commandType == "INVITE")
		return (invite_command(cleanedCommand, fd));
	if (commandType == "/whois" || commandType == "WHOIS")
		return (whois_command(channelName, fd)); //channel name, mais c'est en fait le deuxieme param qui est le nickName dans ce cas
	if(getChannel(channelName) && getChannel(channelName)->isOperator(fd))
	{
		if ((commandType == "/mode" || commandType == "MODE")) {
			std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
			if (it == _channels.end()) {
				// Si le canal n'existe pas, renvoyer un message d'erreur
				sendError(fd, channelName, ERR_NOSUCHCHANNEL);
				return ;
			} else
				return ((*it->second).mode_command(stream, fd));
		}
		// if (commandType == "/kick" || commandType == "KICK")
		// 	return (kick_command(cleanedCommand, fd));
	} else if (commandType == "/invite" || commandType == "INVITE" || commandType == "/kick" || commandType == "KICK"
				|| commandType == "/topic" || commandType == "TOPIC" || commandType == "/mode" || commandType == "MODE")
		return (sendError(fd, _clients[fd]->getNickName(), ERR_CHANOPRIVSNEEDED));
	return (sendError(fd, commandType, ERR_UNKNOWNCOMMAND)); // Si la commande ne correspond à rien
}


