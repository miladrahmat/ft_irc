#pragma once

#include "ACommand.hpp"

class KickCommand : public ACommand {
    private:
        std::string _channel;
        std::string _client_to_kick;
        std::string _msg;
        KickCommand(std::string command, std::shared_ptr<Client>& client, State& state);
    public:
        static std::unique_ptr<ACommand> create(std::string command, std::shared_ptr<Client>& client,
            State& state, std::vector<std::string> args);
        void execute() const override;
};