
#pragma once

#include <string>
#include <vector>
#include <optional>
#include "Client.hpp"

class ACommand {
    protected:
        std::string _command;
        Client & _client;
    
    public:
        ACommand(std::string command, Client & client);
        virtual bool execute() const = 0;
        virtual std::optional<JoinCommand> create(std::string command, Client& client) = 0;
};