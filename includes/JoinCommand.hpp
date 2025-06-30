
#pragma once

#include "ACommand.hpp"
#include <sstream>

class JoinCommand : public ACommand {
    private:
        std::vector<std::string> _channels; //at least one required, otherwise comma separated
        std::vector<std::string> _keys; //optional, comma separated
        JoinCommand(std::string command, std::shared_ptr<Client> & client, State & state);
    
    public:
        static std::unique_ptr<ACommand> create(std::string command, std::shared_ptr<Client>& client, State & state,
            std::vector<std::string> args);
        bool execute() const override;
        static bool validChannelName(std::string channel);
};