#include "QuitCommand.hpp"

QuitCommand::QuitCommand(std::string command, std::shared_ptr<Client>& client, State& state) : ACommand(command, client, state) {}

std::unique_ptr<ACommand>	QuitCommand::create(std::string command, std::shared_ptr<Client>& client, State& state,
	std::string args) {
	QuitCommand*	cmd = new QuitCommand(command, client, state);
    cmd->_msg = args;
    return (std::unique_ptr<QuitCommand>(cmd));
}

void    QuitCommand::execute() const {
    _state.removeClient(_client, _msg);
}