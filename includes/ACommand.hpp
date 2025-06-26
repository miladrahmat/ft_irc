
#pragma once

#include <string>
#include <vector>
#include <optional>
#include <memory>
#include "Channel.hpp"
#include "Client.hpp"
#include "State.hpp"

//Every derived class of ACommand will have a static class create that will look exactly like this:
//  static std::unique_ptr<ACommand> create(std::string command, Client& client, Server & server,
//      std::vector<std::string> args);
//that will be called to create the command objects. This follows some version of the factory
//design pattern.

class ACommand {
    protected:
        std::string _command;
        Client & _client;
        State & _state;
    
    public:
        ACommand(std::string command, Client & client, State & state);
        virtual bool execute() const = 0;
        virtual ~ACommand() = default;
};