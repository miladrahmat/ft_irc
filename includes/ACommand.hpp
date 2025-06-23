
#pragma once

#include <string>
#include <vector>
#include <optional>
#include <memory>
#include "Client.hpp"

class ACommand {
    protected:
        std::string _command;
        Client & _client;
    
    public:
        ACommand(std::string command, Client & client);
        virtual bool execute() const = 0;
        virtual std::unique_ptr<ACommand> create(std::string command, Client& client,
            std::vector<std::string> args) = 0;
};