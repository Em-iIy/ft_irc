# ----------------------------------------Name
NAME = ircserv

# ----------------------------------------Config
PORT = 6667
PASSWORD = hi

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
				VERSION.cpp \

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
HOST_NAME = \"$(shell hostname)\"
DFLAGS += -DHOST_NAME=$(HOST_NAME)

# ----------------------------------------Flags
CC = c++
CFLAGS = -std=c++11
CFLAGS += -Wall -Wextra -Werror
# CFLAGS += -g -fsanitize=address

INC =  -Iinc

# ----------------------------------------Debug
ifdef DEBUG_MODE
CFLAGS += -g -fsanitize=address
DFLAGS += -DDEBUG_MODE=\"1\"
endif

# ----------------------------------------Logs
ifdef LOG_MODE
DFLAGS += -DLOG_MODE
DFLAGS += -DDEBUG_MODE=\"2\"
endif

# ----------------------------------------Making
all:
	echo $(HOST_NAME)
	@$(MAKE) $(NAME) -j4
.PHONY: all

$(NAME): $(DIR_OBJS) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(INC) $(LFLAGS)


$(DIR_OBJS)%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INC) $(DFLAGS)

$(DIR_OBJS):
	mkdir -p $@

# ----------------------------------------Debug
debug:
	$(MAKE) re DEBUG_MODE=1
.PHONY: debug

# ----------------------------------------Logs
log:
	$(MAKE) re LOG_MODE=1
.PHONY: log

# ----------------------------------------Cleaning
clean:
	rm -f $(OBJS)
.PHONY: clean

fclean: clean
	rm -f $(NAME)
.PHONY: fclean

re: fclean all
.PHONY: re


# ----------------------------------------Util
run: all
	./$(NAME) $(PORT) $(PASSWORD)
.PHONY: run

lines:
	wc -l main.cpp src/*.cpp src/cmds/*.cpp inc/*.hpp
.PHONY: lines

test: tests/testConn.cpp
	g++ tests/testConn.cpp -o testConn -DPORT=$(PORT) -DPASSWORD=\"$(PASSWORD)\" && ./testConn
	rm -f testConn
.PHONY: test
