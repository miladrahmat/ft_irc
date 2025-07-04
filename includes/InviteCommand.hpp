#pragma once

#include "ACommand.cpp"

class InviteCommand : public ACommand {
    private:
        InviteCommand(std::string command, std::shared_ptr<Client>& client, State& state);
    public:
        static std::unique_ptr<ACommand> create(std::string command, std::shared_ptr<Client>& client, State& state,
			std::vector<std::string> args);
        bool    execute() const override;
};