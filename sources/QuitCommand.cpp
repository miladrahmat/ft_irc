#include "QuitCommand.hpp"

QuitCommand::QuitCommand(std::string command, std::shared_ptr<Client>& client, State& state) : ACommand(command, client, state) {}

std::unique_ptr<ACommand>	QuitCommand::create(std::string command, std::shared_ptr<Client>& client, State& state,
	std::string args) {
	QuitCommand*	cmd = new QuitCommand(command, client, state);
    cmd->_msg = args;
    return (std::unique_ptr<QuitCommand>(cmd));
}

void    QuitCommand::execute() const {
	std::map<std::string, std::shared_ptr<Client>>	msg_clients;
	Message msg;

	for (auto it = _state.getChannels().begin(); it != _state.getChannels().end(); ++it) {
		if (it->isClient(_client)) {
			for (auto cit = it->clients.begin(); cit != it->clients.end(); ++cit) {
				if (*cit != _client) {
					msg_clients.insert({(*cit)->getNickname(), *cit});
				}
			}
		}
	}
	for (auto it = msg_clients.begin(); it != msg_clients.end(); ++it) {
		msg.message(_client, it->second, _command, {}, _msg);
	}
    _state.removeClient(_client);
}