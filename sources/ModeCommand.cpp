
#include "ModeCommand.hpp"

ModeCommand::ModeCommand(std::string command, std::shared_ptr<Client> & client, State & state) : ACommand(command, client, state), _limit(-1) {}

void ModeCommand::setModeAction(char mode_action) {
    switch (mode_action) {
        case '-': _action = REMOVE; break;
        case '+': _action = ADD; break;
    }
}

void ModeCommand::setMode(char mode) {
    switch (mode) {
        case 'i': _mode = INVITE; break;
        case 't': _mode = TOPIC; break;
        case 'k': _mode = KEY; break;
        case 'o': _mode = OPERATOR; break;
        case 'l': _mode = LIMIT; break;
        default: _mode = UNKNOWN;
    }
}

int ModeCommand::checkTarget(std::string & target) {
    Message msg;
    if (target[0] == '#' || target[0] == '&') {
        if (_state.getChannel(target) != _state.getChannels().end()) {
            _channel_it = _state.getChannel(target);
        }
        else {
            Message msg;
            msg.codedMessage(_client, _state, ERR_NOSUCHCHANNEL, target);
            return (0);
        }
    }
    else if (_state.getClient(target) != _state.getClients().end()) {
        msg.codedMessage(_client, _state, ERR_UMODEUNKNOWNFLAG, target);
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
    cmd->setModeAction(mode[0]);
    cmd->setMode(mode[1]);
    if (cmd->_mode == UNKNOWN) {
        delete (cmd);
        msg.codedMessage(client, cmd->_state, ERR_UNKNOWNMODE, mode);
        return (nullptr);
    }
    if (mode_param) {
        if (cmd->_mode == KEY && cmd->_action == ADD) {
            cmd->_key = *mode_param;
        }
        else if (cmd->_mode == OPERATOR) {
            cmd->_nick = *mode_param;
        }
        else if (cmd->_mode == LIMIT) {
            if (cmd->_action == REMOVE) {
                cmd->_limit = -1;
            }
            else {
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
    }
    if (cmd->_mode == KEY && cmd->_action == ADD && cmd->_key == "") {
        delete (cmd);
        return (nullptr);
    }
    if (cmd->_mode == OPERATOR && cmd->_nick == "") {
        delete (cmd);
        msg.codedMessage(client, cmd->_state, ERR_NEEDMOREPARAMS, command);
        return (nullptr);
    }
    if (cmd->_mode == LIMIT && cmd->_action == ADD && cmd->_limit == -1) {
        delete (cmd);
        msg.codedMessage(client, cmd->_state, ERR_NEEDMOREPARAMS, command);
        return (nullptr);
    }
    return (std::unique_ptr<ModeCommand>(cmd));
}

bool ModeCommand::execute() const {
    Message msg;
    reply reply;
    if (_channel_it->isOperator(_client) == false) {
        msg.codedMessage(_client, _state, ERR_CHANOPRIVSNEEDED, _channel_it->getName());
        return (false);
    }
    switch (_mode) {
        case INVITE:
            executeInvite(*_channel_it);
            return (true);
        case TOPIC:
            executeTopic(*_channel_it);
            return  (true);
        case KEY:
            executeKey(*_channel_it);
            return (true);
        case LIMIT:
            executeLimit(*_channel_it);
            return (true);
        case OPERATOR:
            executeOperator(*_channel_it);
        case UNKNOWN:
            return (false);
    }
    return (true);
}

void ModeCommand::executeKey(Channel & channel) const {
    Message msg;
    reply reply;
    if (_action == REMOVE && channel.getPassword() == "") {
        return;
    }
    reply = channel.setChannelPassword(_client, _key);
    if (reply.code == SUCCESS.code) {
        char action_char = static_cast<char>(_action);
        char mode_char = static_cast<char>(_mode);
        std::string mode = std::string("") + action_char + mode_char;
        std::string key = _key;
        if (_key == "") {
            key = "*";
        }
        msg.message(_client, _client, "MODE", channel.getName() + " " + mode, key);
        channel.sendMsgToAll(_client, "MODE", channel.getName() + " " + mode, key);
    }
    else {
        msg.codedMessage(_client, _state, reply, _channel_it->getName());
    }
}

void ModeCommand::executeInvite(Channel & channel) const {
    Message msg;
    reply reply;
    reply = channel.setInviteMode(_client, (_action == ADD) ? true : false);
    if (reply.code == SUCCESS.code) {
        char action_char = static_cast<char>(_action);
        char mode_char = static_cast<char>(_mode);
        std::string mode = std::string("") + action_char + mode_char;
        msg.message(_client, _client, "MODE", channel.getName() + " " + mode, {});
        channel.sendMsgToAll(_client, "MODE", channel.getName() + " " + mode, {});
    }
    else {
        msg.codedMessage(_client, _state, reply, channel.getName());
    }
}

void ModeCommand::executeTopic(Channel & channel) const {
    Message msg;
    reply reply;
    reply = (*_channel_it).setTopicMode(_client, (_action == ADD) ? true : false);
    if (reply.code == SUCCESS.code) {
        char action_char = static_cast<char>(_action);
        char mode_char = static_cast<char>(_mode);
        std::string mode = std::string("") + action_char + mode_char;
        msg.message(_client, _client, "MODE", channel.getName() + " " + mode, {});
        channel.sendMsgToAll(_client, "MODE", channel.getName() + " " + mode, {});
    }
    else {
        msg.codedMessage(_client, _state, reply, channel.getName());
    }
}

void ModeCommand::executeLimit(Channel & channel) const {
    Message msg;
    reply reply;
    reply = channel.setUserLimit(_client, _limit);
    if (reply.code == SUCCESS.code) {
        char action_char = static_cast<char>(_action);
        char mode_char = static_cast<char>(_mode);
        std::string mode = std::string("") + action_char + mode_char;
        if (_action == REMOVE) {
            msg.message(_client, _client, "MODE", channel.getName() + " " + mode, {});
            channel.sendMsgToAll(_client, "MODE", channel.getName() + " " + mode, {});
        }
        else {
            msg.message(_client, _client, "MODE", channel.getName() + " " + mode, std::to_string(_limit));
            channel.sendMsgToAll(_client, "MODE", channel.getName() + " " + mode, std::to_string(_limit));
        }
    }
    else {
        msg.codedMessage(_client, _state, reply, channel.getName());
    }
}

void ModeCommand::executeOperator(Channel & channel) const {
    Message msg;
    reply reply;
    if (_action == ADD) {
        std::vector<std::shared_ptr<Client>>::iterator new_operator_it = channel.getClient(_nick);
        if (new_operator_it != channel.clients.end()) {
            reply = channel.addOperator(_client, *(new_operator_it));
        }
    }
    else {
        std::vector<std::shared_ptr<Client>>::iterator new_operator_it = channel.getClient(_nick);
        if (new_operator_it != channel.clients.end()) {
            reply = channel.removeOperator(_client, *(new_operator_it));
        }
    }
    if (reply.code == SUCCESS.code) {
        char action_char = static_cast<char>(_action);
        char mode_char = static_cast<char>(_mode);
        std::string mode = std::string("") + action_char + mode_char;
        msg.message(_client, _client, "MODE", channel.getName() + " " + mode, _nick);
        channel.sendMsgToAll(_client, "MODE", channel.getName() + " " + mode, _nick);
    }
    else {
        msg.codedMessage(_client, _state, ERR_NOSUCHNICK, _nick);
    }
}