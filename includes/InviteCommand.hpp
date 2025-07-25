#pragma once

#include "ACommand.hpp"

class InviteCommand : public ACommand {
    private:
        std::string _invited_client;
        std::string _channel;
        InviteCommand(std::string command, std::shared_ptr<Client>& client, State& state);
    public:
        static std::unique_ptr<ACommand> create(std::string command, std::shared_ptr<Client>& client,
            State& state, std::string nick, std::string channel);
        void execute() const override;
};