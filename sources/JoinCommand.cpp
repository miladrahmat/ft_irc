
#include "JoinCommand.hpp"

JoinCommand::JoinCommand(std::string command, Client & client, State & state) : ACommand(command, client, state) {}

bool JoinCommand::execute() const {
    if (_invite_flag) {
        std::vector<Channel>::iterator chan = _state.getChannel(_client.getChannelInvitedTo());
        if (chan != _state.getChannels().end()) {
            return (chan->join(_client, ""));
        }
    }
    std::vector<std::string>::const_iterator chan_it = _channels.begin();
    std::vector<std::string>::const_iterator key_it = _keys.begin();
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
        if (key_it != _keys.end()) {
            key_it++;
        }
    }
    return (true);
}

std::unique_ptr<ACommand> JoinCommand::create(std::string command, Client& client, State & state,
            std::vector<std::string> args) {

    JoinCommand* cmd = new JoinCommand(command, client, state);
    std::vector<std::string>::iterator it = args.begin();
    for ( ; it != args.end() && (*it)[0] == '-'; it++) {
        if (*it == "-window") {
            cmd->_window_flag = true;
        }
        else if (*it == "-invite") {
            cmd->_invite_flag = true;
        }
        else {
            cmd->_server_tag = *it;
        }
    }
    if (it != args.end()) {
        std::stringstream ss(*it);
        for (std::string str; ss >> str;) {
            if (validChannelName(str)) {
                cmd->_channels.push_back(str);
            }
            if (ss.peek() == ',')
                ss.ignore();
        }
    }
    it++;
    if (it != args.end()) {
        std::stringstream ss(*it);
        for (std::string str; ss >> str;) {
            if (str.length() )
            cmd->_keys.push_back(str);    
            if (ss.peek() == ',')
                ss.ignore();
        }
    }
    if (!cmd->_invite_flag && cmd->_channels.empty()) {
        return (nullptr);
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

