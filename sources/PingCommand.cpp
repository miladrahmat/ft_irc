
#include "PingCommand.hpp"

PingCommand::PingCommand(std::string command, std::shared_ptr<Client>& client, State& state) : ACommand(command, client, state) {}

std::unique_ptr<ACommand> PingCommand::create(std::string command, std::shared_ptr<Client>& client, \
		State& state, std::string& args) {
	PingCommand* cmd = new PingCommand(command, client, state);
	if (args[0] == ':') {
		args.erase(0, 1);
	}
	if (args.empty()) {
		Message msg;
		msg.codedMessage(cmd->_client, cmd->_state, ERR_NOORIGIN, {});
		delete cmd;
		return (nullptr);
	}
	if (args.length() > MSG_MAXLEN) {
		cmd->_msg = args.substr(0, MSG_MAXLEN);
	}
	else {
		cmd->_msg = args;
	}
	return (std::unique_ptr<ACommand>(cmd));
}

void	PingCommand::execute() const {
	Message msg;

	msg.messagePong(_client, _state.getServerName(), _command, _state.getServerName(), _msg);
}
