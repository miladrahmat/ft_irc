NAME := ircserv
CC := c++
FLAGS := -Wall -Wextra -Werror -g #REMOVE g

CFILES := main.cpp \

INC := includes/ircserv
SRC_PATH := sources/
OBJ_PATH := objects/

CFILES := $(addprefix $(SRC_PATH), $(CFILES))
OBJS := $(CFILES:$(SRC_PATH)%.c=$(OBJ_PATH)%.o)

all: $(OBJ_PATH) $(NAME)

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)
#	$(OBJ_PATH)/name_of_folder

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(INC)
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all