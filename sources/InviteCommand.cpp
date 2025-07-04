#include "InviteCommand.hpp"

InviteCommand::InviteCommand(std::string command, std::shared_ptr<Client>& client, State& state) : ACommand(command, client, state) {}

std::unique_ptr<ACommand>	InviteCommand::create(std::string command, std::shared_ptr<Client>& client, State& state,
	std::vector<std::string> args) {
	InviteCommand*	cmd = new InviteCommand(command, client, state);
    
    return (std::unique_ptr<InviteCommand>(cmd));
}