
#include "PrivmsgCommand.hpp"

PrivmsgCommand::PrivmsgCommand(std::string command, std::shared_ptr<Client>& client, State& state) : ACommand(command, client, state) {}

std::unique_ptr<ACommand>	PrivmsgCommand::create(std::string command, std::shared_ptr<Client>& client, State& state,
	std::string target, std::string msg) {
		PrivmsgCommand*	cmd = new PrivmsgCommand(command, client, state);

	cmd->_msg_to = target;
	cmd->_msg = msg.substr(1, msg.length());
	if (cmd->_msg_to[0] == '#' || cmd->_msg_to[0] == '&') {
		std::vector<Channel>::iterator channel = cmd->_state.getChannel(cmd->_msg_to);
		if (channel == cmd->_state.getChannels().end()) {
			delete cmd;
			return (nullptr);
		}
		cmd->_channel = true;
	}
	else {
		std::vector<std::shared_ptr<Client>>::iterator client = cmd->_state.getClient(cmd->_msg_to);
		if (client == cmd->_state.getClients().end()) {
			delete cmd;
			return (nullptr);
		}
		cmd->_channel = false;
	}
	return (std::unique_ptr<PrivmsgCommand>(cmd));
}

void	PrivmsgCommand::execute() const {
	Message	msg;

	if (_channel) {
		std::vector<Channel>::iterator channel = _state.getChannel(_msg_to);
		for (auto it = channel->clients.begin(); it != channel->clients.end(); it++) {
			if (_client != *it)
				msg.message(_client, *it, _command, _msg_to, _msg);
			}
		}
	else {
		auto	client = _state.getClient(_msg_to);
		msg.message(_client, *client, _command, _msg_to, _msg);
	}
}