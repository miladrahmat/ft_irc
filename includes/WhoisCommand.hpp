#pragma once

#include "ACommand.hpp"

class WhoisCommand : public ACommand {
    private:
        std::string _server;
        std::string _nick;
        WhoisCommand(std::string command, std::shared_ptr<Client>& client, State& state);
    public:
        static std::unique_ptr<ACommand> create(std::string command, std::shared_ptr<Client>& client,
            State& state, std::string server, std::string nick);
        void execute() const override;
};