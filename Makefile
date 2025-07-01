NAME := ircserv
CC := c++
FLAGS := -Wall -Wextra -Werror -std=c++17 -g #REMOVE g

CFILES := main.cpp \
			Server.cpp \
			Client.cpp \
			Message.cpp \
			Channel.cpp \
			ACommand.cpp \
			JoinCommand.cpp  \
			Parser.cpp \
			State.cpp

INC := includes/
SRC_PATH := sources/
OBJ_PATH := objects/

CFILES := $(addprefix $(SRC_PATH), $(CFILES))
OBJS := $(CFILES:$(SRC_PATH)%.c=$(OBJ_PATH)%.o)

all: $(OBJ_PATH) $(NAME)

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)
#	$(OBJ_PATH)/name_of_folder

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(INC)
	$(CC) $(FLAGS) -I $(INC) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -I $(INC) -o $(NAME)

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all