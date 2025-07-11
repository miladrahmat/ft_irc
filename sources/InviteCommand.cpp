#include "InviteCommand.hpp"

InviteCommand::InviteCommand(std::string command, std::shared_ptr<Client>& client, State& state) : ACommand(command, client, state) {}

std::unique_ptr<ACommand>	InviteCommand::create(std::string command, std::shared_ptr<Client>& client, State& state,
	std::vector<std::string> args) {
	InviteCommand*	cmd = new InviteCommand(command, client, state);
	cmd->_invited_client = args[0];
	cmd->_channel = args[1];
    
    return (std::unique_ptr<InviteCommand>(cmd));
}

void    InviteCommand::execute() const {
    Message msg;
	std::string target;
	reply	code;

    std::vector<Channel>::iterator channel = _state.getChannel(_channel);
	std::vector<std::shared_ptr<Client>>::iterator invited_client = _state.getClient(_invited_client);
	if (invited_client == _state.getClients().end()) {
		code = ERR_NOSUCHNICK;
	}
	else if (channel == _state.getChannels().end()) 
		code = ERR_NOSUCHCHANNEL;
	else {
		code = channel->inviteClient(_client, *invited_client);
	}
	if (code.code == ERR_NOTONCHANNEL.code || code.code == ERR_CHANOPRIVSNEEDED.code || code.code == ERR_NOSUCHCHANNEL.code) {
		target = _channel;
	}
	else if (code.code == ERR_NOSUCHNICK.code) {
		target = _invited_client;
	}
	else if (code.code == ERR_USERONCHANNEL.code || code.code == RPL_INVITING.code) {
		target = _invited_client + " " + _channel;
	}
	msg.codedMessage(_client, _state, code, target);
	if (code.code == RPL_INVITING.code) {
		msg.message(_client, *invited_client, "INVITE", _invited_client, _channel);
	}
}