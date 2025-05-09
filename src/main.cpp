#define PORT av[1]
#define PASSWORD av[2]

#define RED "\001\e[31m\002"
#define GRN "\001\e[32m\002"
#define YEL "\001\e[33m\002"
#define BLU "\001\e[34m\002"
#define PNK "\001\e[35m\002"
#define RST "\001\e[0m\022"

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include "../includes/utils.hpp"

static bool	isValidPassword(std::string password) {
	if (password.size() > 12 || password.size() < 4)
		return (false);
	return (true);
}

static bool	isValidPort(int port, std::string port_string) {
	if (port > 6665 && port < 6669 && port_string.length() == 4)
	{
		return (true);
	}
	return (false);
}

static bool	isValidInput(int ac, char **av){
	int checker = 0;

	if (ac != 3)
		return (std::cerr << "./ircserv <port[6665-6669]> <password[4-10 chars]>\n", false);
	if (!isValidPort(atoi(PORT), PORT))
		std::cerr << "Wrong port\n", checker++;
	if (!isValidPassword(PASSWORD))
		std::cerr << "Wrong password\n", checker++;
	if (checker != 0)
		return (false);
	return (true);
}

int	main(int ac, char **av)
{
	if (!isValidInput(ac, av))
		return (EXIT_FAILURE);
	Server server(atoi(PORT), PASSWORD);
	std::cout << "-_-_-_-_-_-_-_-_-_- Server online -_-_-_-_-_-_-_-_-_-" << std::endl;
	signal(SIGINT, Server::signalHandler);
	signal(SIGQUIT, Server::signalHandler);
	server.serverInit();
	server.setupServer();
	server.acceptClient();
	std::cout << "-_-_-_-_-_-_-_-_-_- Server offline -_-_-_-_-_-_-_-_-_-" << std::endl;
	return (EXIT_SUCCESS);
}

