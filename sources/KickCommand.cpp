#include "KickCommand.hpp"

KickCommand::KickCommand(std::string command, std::shared_ptr<Client>& client, State& state) : ACommand(command, client, state) {}

std::unique_ptr<ACommand>	KickCommand::create(std::string command, std::shared_ptr<Client>& client, State& state,
	std::string args) {
		KickCommand*	cmd = new KickCommand(command, client, state);
    cmd->_msg = args;
    return (std::unique_ptr<KickCommand>(cmd));
}

bool    KickCommand::execute() const {
    _channel.kickClient(_client)
    return (true);
}