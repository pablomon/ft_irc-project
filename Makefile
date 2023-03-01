SRCS			=	app/main.cpp \
					app/Services/Socket.cpp \
					app/Services/Server.cpp \
					app/Model/model.cpp\
					app/Model/channel.cpp \
					app/Model/user.cpp \
					app/Controller/ServicesController.cpp \
					app/Services/Commander/command.cpp \
					app/Services/Commander/nickCmd.cpp \
					app/Services/Commander/passCmd.cpp \
					app/Services/Commander/userCmd.cpp \
					app/Services/Commander/pingCmd.cpp \
					app/Services/Commander/privmsgCmd.cpp \
					app/Services/Commander/joinCmd.cpp \
					app/Services/Commander/quitCmd.cpp \
					app/Services/Commander/partCmd.cpp \
					app/Services/Commander/topicCmd.cpp \
					app/Services/Commander/kickCmd.cpp \
					app/Services/Commander/modeCmd.cpp \
					app/Services/Commander/noticeCmd.cpp \
					app/Services/Commander/whoCmd.cpp \
					app/Services/Commander/killCmd.cpp \
					app/Services/Commander/operCmd.cpp \
					app/Services/Commander/listCmd.cpp \
					app/Services/Commander/dieCmd.cpp \
					app/Templates/parser.cpp \
					app/Templates/welcome.cpp \
					app/Templates/rule.cpp \
					app/Templates/msgTools.cpp

SRCS_BONUS		=	bot/customBot.cpp \
					bot/bot.cpp \
					bot/empeBot.cpp

NAME			= ircserv

NAME_BOT		= botLauncher

CXX				= clang++
RM				= rm -f
CXXFLAGS		= -Wall -Wextra -Werror -std=c++98

OBJS			= $(SRCS:.cpp=.o)

OBJS_BONUS		= $(SRCS_BONUS:.cpp=.o)

$(NAME):		$(OBJS)
		$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

all:			$(NAME)

$(NAME_BOT):	$(OBJS_BONUS)
		$(CXX) $(CXXFLAGS) $(OBJS_BONUS) -o $(NAME_BOT)

bonus:			all $(NAME_BOT)

clean:
				$(RM) $(OBJS) $(OBJS_BONUS)

fclean:			clean
				$(RM) $(NAME) $(NAME_BOT)


re:				fclean all

.PHONY:			all clean fclean re
