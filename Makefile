NAME = ircserv
CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = ./src/main.cpp \
		./src/Server.cpp \
		./src/Client.cpp \
		./src/Channel.cpp \
		./src/join.cpp \
		./src/part.cpp \
		./src/privmsg.cpp \
		./src/kick.cpp \
		./src/invite.cpp \
		./src/topic.cpp \
		./src/mode.cpp \
		./src/utils.cpp \
		./src/nick.cpp \
		./src/parsing.cpp \
		./src/whois.cpp

OBJDIR = ./obj/
OBJS = $(addprefix $(OBJDIR), $(notdir $(SRCS:.cpp=.o)))

RM = rm -f
RMDIR = rm -rf

all : $(NAME)

./obj:
	@mkdir -p ./obj

$(OBJDIR)%.o : ./src/%.cpp | ./obj
	$(CC) $(CPPFLAGS) -c $< -o $@

$(NAME) : $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)

clean :
	$(RM) $(OBJS)
	$(RMDIR) ./obj

fclean : clean
	$(RM) $(NAME)

re : fclean all

# valgrind:
# 	make && valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) 6667 jcock

.PHONY: all clean re fclean
