
#pragma once

#include <string>
#include <vector>
#include <optional>
#include <memory>
#include "Server.hpp"
#include "Client.hpp"

class ACommand {
    protected:
        std::string _command;
        Client & _client;
        Server & _server;
    
    public:
        ACommand(std::string command, Client & client, Server & server);
        virtual bool execute() const = 0;
        static std::unique_ptr<ACommand> create(std::string command, Client& client,
            std::vector<std::string> args);
        virtual ~ACommand() = default;
};