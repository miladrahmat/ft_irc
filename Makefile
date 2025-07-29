NAME := ircserv
CC := c++
FLAGS := -Wall -Wextra -Werror -std=c++17 -MMD -g #REMOVE g

CFILES := main.cpp \
			Server.cpp \
			Client.cpp \
			Message.cpp \
			Channel.cpp \
			ACommand.cpp \
			JoinCommand.cpp  \
			PrivmsgCommand.cpp \
			ModeCommand.cpp \
			NickCommand.cpp \
			Parser.cpp \
			State.cpp \
			QuitCommand.cpp \
			KickCommand.cpp \
			InviteCommand.cpp \
			TopicCommand.cpp \
			WhoisCommand.cpp

INC := includes/
SRC_PATH := sources/
OBJ_PATH := objects_and_dependencies/

CFILES := $(addprefix $(SRC_PATH), $(CFILES))
OBJS := $(CFILES:$(SRC_PATH)%.cpp=$(OBJ_PATH)%.o)
DEPS := $(OBJS:.o=.d)

GREEN := \e[1;92m
CYAN := \e[1;36m
RED := \e[1;31m
YELLOW := \e[1;93m
RESET := \e[0m

COMPILING_OBJ := echo "$(YELLOW)⏳ Compiling object files..$(RESET)"
CLEANING := echo "$(CYAN)💧 Cleaning...$(RESET)"
CLEANED := echo "$(CYAN)☀️  Successfully cleaned all object and dependency files!$(RESET)"
FCLEANING := echo "$(CYAN)💧 Deep cleaning...$(RESET)"
FCLEANED := echo "$(CYAN)☀️  Successfully cleaned all executable files!$(RESET)"
PROG_READY := echo "$(GREEN)💎 IRC Server GALLERIA ready! 💎$(RESET)"

CLEAN_ERR := echo "$(RED)❌ An error occured while cleaning... Aborting... ❌$(RESET)"
FCLEAN_ERR := echo "$(RED)❌ An error occured while deep cleaning... Aborting ❌$(RESET)"
PROG_ERR := echo "$(RED)❌ An error occured while compiling the program... Aborting ❌$(RESET)"

all: $(OBJ_PATH) $(NAME)

-include $(DEPS)

$(OBJ_PATH):
	@$(COMPILING_OBJ)
	@mkdir -p $(OBJ_PATH)
#	$(OBJ_PATH)/name_of_folder

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp $(INC)
	@echo "$(YELLOW)⏳ Compiling $@...$(RESET)"
	@$(CC) $(FLAGS) -I $(INC) -c $< -o $@ \
	|| echo "$(RED)😔 Unable to compile $@$(RESET)"

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -I $(INC) -o $(NAME) \
	&& $(PROG_READY) \
	|| $(PROG_ERR)


clean:
	@rm -rf $(OBJ_PATH) \
	&& $(CLEANED) \
	|| $(CLEAN_ERR)

fclean: clean
	@rm -f $(NAME) \
	&& $(FCLEANED) \
	|| $(FCLEAN_ERR)

re: fclean all

.PHONY: all clean fclean re