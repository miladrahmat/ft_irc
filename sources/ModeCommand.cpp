
#include "ModeCommand.hpp"

ModeCommand::ModeCommand(std::string command, std::shared_ptr<Client> & client, State & state) : ACommand(command, client, state), _limit(-1) {}

void ModeCommand::setModeAction(char mode_action) {
    switch (mode_action) {
        case REMOVE: _action = REMOVE;
        case ADD: _action = ADD;
    }
}

void ModeCommand::setMode(char mode) {
    switch (mode) {
        case INVITE: _mode = INVITE;
        case TOPIC: _mode = TOPIC;
        case KEY: _mode = KEY;
        case OPERATOR: _mode = OPERATOR;
        case LIMIT: _mode = LIMIT;
        default: _mode = UNKNOWN;
    }
}

int ModeCommand::checkTarget(std::string & target) {
    if (_state.getClient(target) != _state.getClients().end()) {
        _target = target;
    }
    else if (_state.getChannel(target) != _state.getChannels().end()) {
        _target = target;
    }
    else {
        Message msg;
        msg.codedMessage(_client, ERR_NOSUCHNICK, target);
        return (0);
    }
    return (1);
}

std::unique_ptr<ACommand> ModeCommand::create(std::string command, std::shared_ptr<Client>& client,
    State & state, std::string target, std::string mode, std::optional<std::string> mode_param) {
    
    ModeCommand* cmd = new ModeCommand(command, client, state);
    Message msg;
    if (cmd->checkTarget(target) == 0) {
        delete (cmd);
        return (nullptr);
    }
    std::cout << "Target: " << cmd->_target << std::endl;       //remove this
    cmd->setModeAction(mode[0]);
    cmd->setMode(mode[1]);
    if (cmd->_mode == UNKNOWN) {
        delete (cmd);
        reply reply = ERR_UNKNOWNERROR;
        reply.msg = "Unsupported mode";
        msg.codedMessage(client, reply, "MODE " + mode);
        return (nullptr);
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
                int limit = std::stoi(*mode_param);
                if (limit <= 0) {
                    delete (cmd);
                    return (nullptr);
                }
                cmd->_limit = limit;
            }
            catch (std::exception & e) {
                delete (cmd);
                return (nullptr);
            }
        }
    }
    if (cmd->_mode == KEY && cmd->_action == ADD && cmd->_key == "") {
        delete (cmd);
        return (nullptr);
    }
    if (cmd->_mode == OPERATOR && cmd->_nick == "") {
        delete (cmd);
        msg.codedMessage(client, ERR_NEEDMOREPARAMS, command);
        return (nullptr);
    }
    if (cmd->_mode == LIMIT && cmd->_action == ADD && cmd->_limit == -1) {
        delete (cmd);
        msg.codedMessage(client, ERR_NEEDMOREPARAMS, command);
        return (nullptr);
    }
    return (std::unique_ptr<ModeCommand>(cmd));
}

bool ModeCommand::execute() const {
    if (_mode == INVITE) {
        std::vector<Channel>::iterator chan_it = _state.getChannel(_target);
        if (chan_it != _state.getChannels().end()) {
            (*chan_it).setInviteMode(_client, (_action == ADD) ? true : false);
        }
    }
}