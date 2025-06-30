
#include "JoinCommand.hpp"

#include <iostream>

JoinCommand::JoinCommand(std::string command, std::shared_ptr<Client> & client, State & state) : ACommand(command, client, state) {}

bool JoinCommand::execute() const {
    std::vector<std::string>::const_iterator chan_it = _channels.begin();
    std::vector<std::string>::const_iterator key_it = _keys.begin();
    Message msg;
    for ( ; chan_it != _channels.end(); chan_it++) {
        std::vector<Channel>::iterator chan = _state.getChannel(*chan_it);
        if (chan != _state.getChannels().end()) {
            if (key_it == _keys.end()) {
                chan->join(_client, "");
            }
            else {
                chan->join(_client, *key_it);
            }
        }
        else {
            if (key_it == _keys.end()) {
                _state.addNewChannel(*chan_it, _client);
            }
            else {
                _state.addNewChannel(*chan_it, _client, *key_it);
            }
        }
        msg.message(_client, _command, *chan_it, {});
        if (key_it != _keys.end()) {
            key_it++;
        }
    }
    return (true);
}

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

