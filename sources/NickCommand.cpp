
#include "NickCommand.hpp"

NickCommand::NickCommand(std::string command, std::shared_ptr<Client>& client, State& state) : ACommand(command, client, state) {}

std::unique_ptr<ACommand> NickCommand::create(std::string command, std::shared_ptr<Client>& client, State& state,
	std::string nick) {
	NickCommand*	cmd = new NickCommand(command, client, state);
	std::string invalid_start = "$:#&~@+%";
	std::string	invalid = " ,*?!@.";

	cmd->_nickname = nick;
	cmd->_error = false;
	if (cmd->_nickname.empty()) {
		cmd->_reply = ERR_NONICKNAMEGIVEN;
		cmd->_error = true;
	}
	else if (cmd->_nickname.length() > 15) {
		cmd->_reply = ERR_ERRONEUSNICKNAME;
		cmd->_error = true;
	}
	else if (invalid_start.find(cmd->_nickname[0]) != std::string::npos) {
		cmd->_reply = ERR_ERRONEUSNICKNAME;
		cmd->_error = true;
	}
	else {
		for (size_t i = 0; i < invalid.length(); i++) {
			if (cmd->_nickname.find(invalid[i]) != std::string::npos) {
				cmd->_reply = ERR_ERRONEUSNICKNAME;
				cmd->_error = true;
			}
		}
	}
	for (auto it = cmd->_state.getClients().begin(); it != cmd->_state.getClients().end(); ++it) {
		if ((*it)->getNickname() == cmd->_nickname) {
			cmd->_reply = ERR_NICKNAMEINUSE;
			cmd->_error = true;
			break ;
		}
	}
	return (std::unique_ptr<NickCommand>(cmd));
}

void	NickCommand::execute() const {
	Message	msg;

	if (_error) {
		if (_reply.code == ERR_NONICKNAMEGIVEN.code) {
			msg.codedMessage(_client, _state, _reply, {});
		}
		else {
			msg.codedMessage(_client, _state, _reply, _nickname);
		}
		return ;
	}
	if (!_client->isAuthenticated()) {
		_client->setNickname(_nickname);
	}
	else {
		msg.message(_client, _client, _command, {}, _nickname);
		for (auto it = _state.getChannels().begin(); it != _state.getChannels().end(); ++it) {
			if (it->isClient(_client)) {
				it->sendMsgToAll(_client, _command, {}, _nickname);
			}
		}
		_client->setNickname(_nickname);
	}
	_client->validateNick();
}