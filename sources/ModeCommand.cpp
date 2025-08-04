
#include "ModeCommand.hpp"

ModeCommand::ModeCommand(std::string command, std::shared_ptr<Client> & client, State & state) :
    ACommand(command, client, state) {}

void ModeCommand::setModeAction(mode_struct & mode, char mode_action) {
    switch (mode_action) {
        case '-': mode.action = REMOVE; break;
        case '+': mode.action = ADD; break;
    }
}

void ModeCommand::setMode(mode_struct & mode_struct, char mode) {
    switch (mode) {
        case 'i': mode_struct.mode = INVITE; break;
        case 't': mode_struct.mode = TOPIC; break;
        case 'k': mode_struct.mode = KEY; break;
        case 'o': mode_struct.mode = OPERATOR; break;
        case 'l': mode_struct.mode = LIMIT; break;
        default: mode_struct.mode = UNKNOWN;
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
        if (target != _client->getNickname()) {
            msg.codedMessage(_client, _state, ERR_USERSDONTMATCH, target);
            return (0);
        }
        reply message = ERR_UMODEUNKNOWNFLAG;
        message.msg += ". Supported user modes: (none)";
        msg.codedMessage(_client, _state, message, target);
        return (0);
    }
    else {
        msg.codedMessage(_client, _state, ERR_UNKNOWNMODE, target);
        return (0);
    }
    return (1);
}

std::unique_ptr<ACommand> ModeCommand::create(std::string command, std::shared_ptr<Client>& client,
    State & state, std::string target, std::string modes, std::string params) {
    
    ModeCommand* cmd = new ModeCommand(command, client, state);
    Message msg;
    if (cmd->checkTarget(target) == 0) {
        delete (cmd);
        return (nullptr);
    }
    if (modes == "") {
        return (std::unique_ptr<ModeCommand>(cmd));
    }
	char action = '+';
    for ( ; !modes.empty(); modes.erase(0, 1)) {
        if (modes[0] == '+')  {
            action = '+';
            continue ;
        }
        else if (modes[0] == '-') {
            action = '-';
            continue;
        }
        else {
            std::string mode = std::string(1, action) + modes[0];
            std::string param = "";
            mode_struct mode_obj;
            cmd->setModeAction(mode_obj, action);
            cmd->setMode(mode_obj, mode[1]);
            mode_obj.param = param;
            if (mode_obj.mode == UNKNOWN) {
                msg.codedMessage(client, cmd->_state, ERR_UNKNOWNMODE, mode);
                continue;
            }
            if (mode == "+k" || mode[1] == 'o' || mode == "+l") {
                if (params.empty()) {
                    Message msg;
                    msg.codedMessage(client, state, ERR_NEEDMOREPARAMS, command);
                    continue;
                }
                param = params.substr(0, params.find_first_of(' '));
                params.erase(0, param.length() + 1);
                if (param != "") {
                    if (mode_obj.mode == LIMIT && mode_obj.action == REMOVE) {
                        mode_obj.param = "-1";
                    }
                    else {
                        mode_obj.param = param;
                    }
                }
            }
            cmd->modes.push_back(mode_obj);
        }
    }
    if (cmd->modes.empty()) {
        delete (cmd);
        return (nullptr);
    }
    return (std::unique_ptr<ModeCommand>(cmd));
}

void ModeCommand::execute() const {
    Message msg;
    reply reply;
    if (modes.empty()) {
        executeEmptyMode(*(_channel_it));
        return ;
    }
    if (_channel_it->isOperator(_client) == false) {
        msg.codedMessage(_client, _state, ERR_CHANOPRIVSNEEDED, _channel_it->getName());
        return ;
    }
    for (auto mode = modes.begin(); mode != modes.end(); mode++) {
        switch (mode->mode) {
            case KEY:
                executeKey(*_channel_it, *mode);
                continue;
            case INVITE:
                executeInvite(*_channel_it, *mode);
                continue;
            case TOPIC:
                executeTopic(*_channel_it, *mode);
                continue;
            case LIMIT:
                executeLimit(*_channel_it, *mode);
                continue;
            case OPERATOR:
                executeOperator(*_channel_it, *mode);
                continue;
            case UNKNOWN:
                continue;
        }
    }
}

void ModeCommand::executeEmptyMode(Channel & channel) const {
    Message msg;

    std::string message_to_send = channel.getName() + " ";
    message_to_send += channel.getModes(_client);
    msg.codedMessage(_client, _state, RPL_CHANNELMODEIS, message_to_send);
}

void ModeCommand::executeKey(Channel & channel, const mode_struct & mode_obj) const {
    Message msg;
    reply reply;
    if (mode_obj.action == REMOVE && channel.getPassword() == "") {
        return;
    }
    reply = channel.setChannelPassword(_client, mode_obj.param);
    if (reply.code == SUCCESS.code) {
        char action_char = static_cast<char>(mode_obj.action);
        char mode_char = static_cast<char>(mode_obj.mode);
        std::string key = mode_obj.param;
        if (mode_obj.param == "") {
            key = "*";
        }
        std::string mode = std::string("") + action_char + mode_char;
        msg.message(_client, _client, _command, channel.getName() + " " + mode, key);
        channel.sendMsgToAll(_client, _command, channel.getName() + " " + mode, key);
    }
    else {
        msg.codedMessage(_client, _state, reply, _channel_it->getName());
    }
}

void ModeCommand::executeInvite(Channel & channel, const mode_struct & mode_obj) const {
    Message msg;
    reply reply;
    reply = channel.setInviteMode(_client, (mode_obj.action == ADD) ? true : false);
    if (reply.code == SUCCESS.code) {
        char action_char = static_cast<char>(mode_obj.action);
        char mode_char = static_cast<char>(mode_obj.mode);
        std::string mode = std::string("") + action_char + mode_char;
        msg.message(_client, _client, _command, channel.getName() + " " + mode, {});
        channel.sendMsgToAll(_client, _command, channel.getName() + " " + mode, {});
    }
    else {
        msg.codedMessage(_client, _state, reply, channel.getName());
    }
}

void ModeCommand::executeTopic(Channel & channel, const mode_struct & mode_obj) const {
    Message msg;
    reply reply;
    reply = (*_channel_it).setTopicMode(_client, (mode_obj.action == ADD) ? true : false);
    if (reply.code == SUCCESS.code) {
        char action_char = static_cast<char>(mode_obj.action);
        char mode_char = static_cast<char>(mode_obj.mode);
        std::string mode = std::string("") + action_char + mode_char;
        msg.message(_client, _client, _command, channel.getName() + " " + mode, {});
        channel.sendMsgToAll(_client, _command, channel.getName() + " " + mode, {});
    }
    else {
        msg.codedMessage(_client, _state, reply, channel.getName());
    }
}

void ModeCommand::executeLimit(Channel & channel, const mode_struct & mode_obj) const {
    Message msg;
    reply reply;
    int limit;
    if (mode_obj.action == ADD) {
        try {
            limit = std::stoi(mode_obj.param);
            if (limit <= 0) {
                return;
            }
        }
        catch (std::exception & e) {
            std::cerr << e.what() << std::endl;
            return ;
        }
    }
    else {
        limit = -1;
    }
    int limit_before = channel.getUserLimit();
    reply = channel.setUserLimit(_client, limit);
    if (reply.code == SUCCESS.code) {
        char action_char = static_cast<char>(mode_obj.action);
        char mode_char = static_cast<char>(mode_obj.mode);
        std::string mode = std::string("") + action_char + mode_char;
        if (mode_obj.action == REMOVE && limit_before != -1) {
            msg.message(_client, _client, _command, channel.getName() + " " + mode, {});
            channel.sendMsgToAll(_client, _command, channel.getName() + " " + mode, {});
        }
        else if (mode_obj.action == ADD) {
            msg.message(_client, _client, _command, channel.getName() + " " + mode, mode_obj.param);
            channel.sendMsgToAll(_client, _command, channel.getName() + " " + mode, mode_obj.param);
        }
    }
    else {
        msg.codedMessage(_client, _state, reply, channel.getName());
    }
}

void ModeCommand::executeOperator(Channel & channel, const mode_struct & mode_obj) const {
    Message msg;
    reply reply;
    std::vector<std::shared_ptr<Client>>::iterator new_operator_it = channel.getClient(mode_obj.param);
    if (new_operator_it == channel.clients.end()) {
        if (_state.getClient(mode_obj.param) == _state.getClients().end()) {
            msg.codedMessage(_client, _state, ERR_NOSUCHNICK, mode_obj.param);
        }
        else {
            msg.codedMessage(_client, _state, ERR_USERNOTINCHANNEL, mode_obj.param + " " + channel.getName());
        }
        return ;
    }
    if (mode_obj.action == ADD) {
        reply = channel.addOperator(_client, *(new_operator_it));
    }
    else {
        reply = channel.removeOperator(_client, *(new_operator_it));
    }
    if (reply.code == SUCCESS.code) {
        char action_char = static_cast<char>(mode_obj.action);
        char mode_char = static_cast<char>(mode_obj.mode);
        std::string mode = std::string("") + action_char + mode_char;
        msg.message(_client, _client, _command, channel.getName() + " " + mode, mode_obj.param);
        channel.sendMsgToAll(_client, _command, channel.getName() + " " + mode, mode_obj.param);
    }
    else {
        msg.codedMessage(_client, _state, reply, mode_obj.param);
    }
}