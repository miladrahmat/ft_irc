#pragma once

#include "ACommand.hpp"

class KickCommand : public ACommand {
    private:
        Channel &   _channel;
        KickCommand(std::string command, std::shared_ptr<Client>& client, State& state);
    public:
        static std::unique_ptr<ACommand> create(std::string command, std::shared_ptr<Client>& client, State& state,
			std::string args);
        bool    execute() const override;
};