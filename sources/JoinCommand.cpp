
#include "JoinCommand.hpp"

JoinCommand::JoinCommand(std::string command, Client & client) : ACommand(command, client) {}

bool ACommand::execute() const {

}

std::unique_ptr<ACommand> JoinCommand::create(std::string command, Client& client,
            std::vector<std::string> args) {

    std::unique_ptr<JoinCommand> cmd = std::make_unique<JoinCommand>(command, client);
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
    if (!_invite_flag && _channels.empty()) {
        return (nullptr);
    }
    return (cmd);
}

bool JoinCommand::validChannelName(std::string channel_name) {
    if (channel_name[0] != '#') {
        return (false);
    }
    if (channel_name.length() <= 4 && channel_name.length() >= 50) {
        return (false);
    }
    std::string name = channel_name.substr(1);
    for (char c : name) {
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_') {
            continue;
        } else {
            return (false);
        }
    }
    return (true);
}

