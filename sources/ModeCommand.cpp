
#include "ModeCommand.hpp"

ModeCommand::ModeCommand(std::string command, std::shared_ptr<Client> & client, State & state) : ACommand(command, client, state) {}

std::unique_ptr<ACommand> ModeCommand::create(std::string command, std::shared_ptr<Client>& client,
    State & state, std::string target, std::string mode, std::optional<std::string> mode_param) {
    
    ModeCommand* cmd = new ModeCommand(command, client, state);
    //check if the target is channel or nick, and if that exists
    cmd->_target = target;
    std::cout << "Target: " << cmd->_target << std::endl;
    switch (mode[0]) {
        case REMOVE: cmd->_action = REMOVE;
        case ADD: cmd->_action = ADD;
    }
    switch (mode[1]) {
        case INVITE: cmd->_mode = INVITE;
        case TOPIC: cmd->_mode = TOPIC;
        case KEY: cmd->_mode = KEY;
        case OPERATOR: cmd->_mode = OPERATOR;
        case LIMIT: cmd->_mode = LIMIT;
    }
    if (mode_param) {
        if (cmd->_mode == KEY) {
            cmd->_key = *mode_param;
        }
        else if (cmd->_mode == OPERATOR) {
            cmd->_nick = *mode_param;
        }
        else if (cmd->_mode == LIMIT) {
            try {
                cmd->_limit = std::stoul(*mode_param);
            }
            catch (std::exception & e) {
                //delete cmd
                //TODO send error message
            }
        }
    }
}