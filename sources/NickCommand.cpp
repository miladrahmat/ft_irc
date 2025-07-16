
#include "NickCommand.hpp"

NickCommand::NickCommand(std::string command, std::shared_ptr<Client>& client, State& state) : ACommand(command, client, state) {}

std::unique_ptr<ACommand> NickCommand::create(std::string command, std::shared_ptr<Client>& client, State& state,
	std::string nick) {
	NickCommand*	cmd = new NickCommand(command, client, state);
	Message msg;
	std::string invalid_start = "$:#&~@+%";
	std::string	invalid = " ,*?!@.";

	cmd->_nickname = nick;
	if (cmd->_nickname.empty()) {
		msg.codedMessage(cmd->_client, cmd->_state, ERR_NONICKNAMEGIVEN, {});
		delete cmd;
		return (nullptr);
	}
	else if (cmd->_nickname.length() > 15 || invalid_start.find(cmd->_nickname[0]) != std::string::npos) {
		msg.codedMessage(cmd->_client, cmd->_state, ERR_ERRONEUSNICKNAME, cmd->_nickname);
		delete cmd;
		return (nullptr);
	}
	for (size_t i = 0; i < invalid.length(); i++) {
		if (cmd->_nickname.find(invalid[i]) != std::string::npos) {
			msg.codedMessage(cmd->_client, cmd->_state, ERR_ERRONEUSNICKNAME, cmd->_nickname);
			delete cmd;
			return (nullptr);
		}
	}
	for (auto it = cmd->_state.getClients().begin(); it != cmd->_state.getClients().end(); ++it) {
		if ((*it)->getNickname() == cmd->_nickname && *it != cmd->_client) {
			msg.codedMessage(cmd->_client, cmd->_state, ERR_NICKNAMEINUSE, cmd->_nickname);
			delete cmd;
			return (nullptr);
		}
	}
	return (std::unique_ptr<NickCommand>(cmd));
}

void	NickCommand::execute() const {
	Message	msg;

	if (_client->getNickname() == _nickname)
		return ;
	if (_client->isAuthenticated()) {
		msg.message(_client, _client, _command, {}, _nickname);
		for (auto it = _state.getChannels().begin(); it != _state.getChannels().end(); ++it) {
			if (it->isClient(_client)) {
				it->sendMsgToAll(_client, _command, {}, _nickname);
			}
		}
	}
	_client->setNickname(_nickname);
	_client->validateNick();
}