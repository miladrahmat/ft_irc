
#include "JoinCommand.hpp"

#include <iostream>

JoinCommand::JoinCommand(std::string command, std::shared_ptr<Client> & client, State & state) : ACommand(command, client, state) {}

std::unique_ptr<ACommand> JoinCommand::create(std::string command, std::shared_ptr<Client>& client, State & state,
            std::vector<std::string> args) {

    JoinCommand* cmd = new JoinCommand(command, client, state);
    std::vector<std::string>::iterator it = args.begin();
    std::stringstream ss(*it);
    while (ss.good()) {
        std::string substr;
        std::getline(ss, substr, ',');
        if (validChannelName(substr)) {
            cmd->_channels.push_back(substr);
        }
    }
    it++;
    if (it != args.end()) {
        std::stringstream ss(*it);

        while (ss.good()) {
            std::string substr;
            std::getline(ss, substr, ',');
            cmd->_keys.push_back(substr);
        }
    }
    return (std::unique_ptr<JoinCommand>(cmd));
}

bool JoinCommand::validChannelName(std::string channel_name) {
    if (channel_name[0] != '#' && channel_name[0] != '&' ) {
        return (false);
    }
    if (channel_name.length() >= 50) {
        //ERR_INPUTTOOLONG (417)
        return (false);
    }
    std::string name = channel_name.substr(1);
    for (char c : name) {
        if ( c == ' ' || c == ',' || c == 0x07) {

            return (false);
        }
    }
    return (true);
}

void JoinCommand::execute() const {
    std::vector<std::string>::const_iterator chan_it = _channels.begin();
    std::vector<std::string>::const_iterator key_it = _keys.begin();
    reply reply;
    for ( ; chan_it != _channels.end(); chan_it++) {
        std::vector<Channel>::iterator chan = _state.getChannel(*chan_it);
        if (chan != _state.getChannels().end()) {
            if (key_it == _keys.end()) {
                reply = chan->join(_client, "");
            }
            else {
                reply = chan->join(_client, *key_it);
            }
        }
        else {
            if (key_it == _keys.end()) {
                reply = _state.addNewChannel(*chan_it, _client);
            }
            else {
                reply = _state.addNewChannel(*chan_it, _client, *key_it);
            }
        }
        if (reply.code == SUCCESS.code) {
            joinReply(*chan_it);
        }
        else {
            errReply(reply, *chan_it);
        }
        if (key_it != _keys.end()) {
            key_it++;
        }
    }
}

void JoinCommand::joinReply(std::string channel) const {
    Message msg;
    std::vector<Channel>::iterator chan_it = _state.getChannel(channel);
    for (auto client : (*chan_it)._clients) {
        msg.message(_client, client, "JOIN", channel, {});
    }
    if (chan_it->getTopic() != "") {
        struct reply reply = RPL_TOPIC;
        reply.msg = chan_it->getTopic();
        msg.codedMessage(_client, _state, reply, channel);
    }
    std::vector<std::shared_ptr<Client>>::iterator it = (*chan_it)._clients.begin();
    std::string nicks;
    for (int i = 0; it != (*chan_it)._clients.end(); it++, i++) {
        if (chan_it->isOperator(*it)) {
            nicks.append("@");
        }
        nicks.append((*it)->getNickname() + " ");
        if (i == 10) {
            struct reply reply = RPL_NAMREPLY;
            reply.msg = nicks;
            std::string target = "= " + (*chan_it).getName();
            msg.codedMessage(_client, _state, reply, target);
            i = 0;
            nicks.clear();
        }
    }
    struct reply reply = RPL_NAMREPLY;
    reply.msg = nicks;
    std::string target = "= " + (*chan_it).getName();
    msg.codedMessage(_client, _state, reply, target);
    nicks.clear();
    msg.codedMessage(_client, _state, RPL_ENDOFNAMES, (*chan_it).getName());
}

void JoinCommand::errReply(reply reply, std::string channel) const {
    Message msg;
    //std::vector<Channel>::iterator chan_it = _state.getChannel(channel);
    msg.codedMessage(_client, _state, reply, channel);
}
