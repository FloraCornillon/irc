#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"

void Channel::mode_command(std::istringstream &stream, int fd)
{
	std::string	mode;
	std::string	ARG;
	while (stream >> mode)
	{
		if (mode[0] == '+')
		{
			if (mode[1] == 'i')
			{
				setMode('i', true, fd);
			}
			else if (mode == "+t")
				setMode('t', true, fd);
			else if (mode == "+k")
			{
				if (stream >> ARG)
				{
					setPassword(ARG);
					setMode('k', true, fd);
				}
			}
			else if (mode == "+l")
			{
				if (stream >> ARG && ARG.size() <= 6 && atoi(ARG.c_str()) > 0)
				{
					setMode('l', true, fd);
					setMaxUsers(atoi(ARG.c_str()));
				}
			}
			else if (mode == "+o")
			{
				if (stream >> ARG)
				{
					for (std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); ++it)
					{
						if ((*it)->getNickName() == ARG)
						{
							addOperator((*it));
							break;
						}
					}
				}
			}
			else
			{
				_server->sendError(fd, "", ERR_NEEDMOREPARAMS);
				return;
			}
		}
		else if (mode[0] == '-')
		{
			if (mode == "-i")
				setMode('i', false, fd);
			else if (mode == "-t")
				setMode('t', false, fd);
			else if (mode == "-k")
			{
				setPassword("");
				setMode('k', false, fd);
			}
			else if (mode == "-l")
				setMode('l', false, fd);
			else if (mode == "-o")
			{
				if (stream >> ARG)
				{
					for (std::vector<Client*>::iterator it = _operateurs.begin(); it != _operateurs.end(); ++it)
					{
						if ((*it)->getNickName() == ARG)
						{
							removeOperator((*it));
							break;
						}
					}
				}
			}
		}
		else
		{
			std::string modes_help = 
			"The available modes for this channel are:\n"
			"+i : Private channel (visible only to invited users)\n"
			"+t : Topic can only be modified by operators\n"
			"+k : Password-protected channel\n"
			"+l : Limit on the number of members in the channel\n"
			"-i : Disable private channel mode\n"
			"-t : Allow anyone to modify the topic\n"
			"-k : Remove the password requirement to join the channel\n"
			"-l : Remove the member limit restriction\n";
		if (_server)
			_server->sendToClient(fd, modes_help);
		return;
		}
	}
}