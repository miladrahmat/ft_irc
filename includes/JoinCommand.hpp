
#pragma once

#include "ACommand.hpp"
#include <sstream>

class JoinCommand : public ACommand {
    private:
        bool _window_flag; //optional     //TODO how do we handle?
        bool _invite_flag; //optional
        std::string _server_tag; //optional     //TODO how do we handle?
        std::vector<std::string> _channels; //at least one required, otherwise comma separated
        std::vector<std::string> _keys; //optional, comma separated
        JoinCommand(std::string command, Client & client);
    
    public:
        std::unique_ptr<ACommand> create(std::string command, Client& client,
            std::vector<std::string> args) override;
        bool execute() const override;
        bool validChannelName(std::string channel);
};