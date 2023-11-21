# ----------------------------------------Name
NAME = ircserv

# ----------------------------------------Files
FILES_SRCS =	main.cpp \
				Server.cpp \
				Socket.cpp \
				Log.cpp \
				User.cpp \
				Config.cpp \
				serverCmds.cpp \
				utils.cpp \
				Message.cpp \
				AWAY.cpp \
				CAP.cpp \
				MODE.cpp \
				NICK.cpp \
				PASS.cpp \
				PING.cpp \
				QUIT.cpp \
				USER.cpp \

FILES_OBJS = $(FILES_SRCS:.cpp=.o)

# ----------------------------------------Directories
DIR_SRCS = ./src/
DIR_OBJS = ./obj/

vpath %.cpp $(DIR_SRCS) \
			$(DIR_SRCS)cmds/

# ----------------------------------------Sources
SRCS = $(FILES_SRCS:%=$(DIR_SRCS)%)

# ----------------------------------------Objects
OBJS = $(FILES_OBJS:%=$(DIR_OBJS)%)

# ----------------------------------------Defines
HOST_NAME = \"$(shell hostname)\";
DFLAGS += -DHOST_NAME=$(HOST_NAME)

# ----------------------------------------Flags
CC = c++
CFLAGS = -std=c++11
# CFLAGS += -Wall -Wextra -Werror
CFLAGS += -g -fsanitize=address

INC =  -Iinc

# ----------------------------------------Making
all:
	echo $(HOST_NAME)
	@$(MAKE) $(NAME) -j4


$(NAME): $(DIR_OBJS) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(INC) $(LFLAGS)


$(DIR_OBJS)%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INC) $(DFLAGS)

$(DIR_OBJS):
	mkdir -p $@


# ----------------------------------------Cleaning
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all


# ----------------------------------------Util
lines:
	wc -l main.cpp src/*.cpp src/cmds/*.cpp inc/*.hpp

.PHONY: all clean fclean re lines