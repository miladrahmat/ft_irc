#pragma once

#include "ACommand.hpp"

class QuitCommand : public ACommand {
    private:
        std::string _msg;
        QuitCommand(std::string command, std::shared_ptr<Client>& client, State& state);
    public:
        static std::unique_ptr<ACommand> create(std::string command, std::shared_ptr<Client>& client, State& state,
			std::string args);
        bool    execute() const override;
};