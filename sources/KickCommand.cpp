#include "KickCommand.hpp"

KickCommand::KickCommand(std::string command, std::shared_ptr<Client>& client, State& state) : ACommand(command, client, state) {}

std::unique_ptr<ACommand> KickCommand::create(std::string command, std::shared_ptr<Client>& client, State& state,
	std::string channel, std::string nick, std::string message) {
	KickCommand*	cmd = new KickCommand(command, client, state);
    cmd->_channel = channel;
    cmd->_client_to_kick = nick;
    cmd->_msg = message;
    return (std::unique_ptr<KickCommand>(cmd));
}


void KickCommand::execute() const {
    Message msg;
    std::vector<Channel>::iterator channel = _state.getChannel(_channel);
    std::vector<std::shared_ptr<Client>>::iterator client_to_kick = _state.getClient(_client_to_kick);
    reply code;
    if (channel == _state.getChannels().end()) {
        code = ERR_NOSUCHCHANNEL;
    }
    else if (!channel->isClient(_client)) {
        code = ERR_NOTONCHANNEL;
    }
    else if (!channel->isOperator(_client)) {
        code = ERR_CHANOPRIVSNEEDED;
    }
    else if (client_to_kick == _state.getClients().end()) {
        code = ERR_NOSUCHNICK;
    }
    else {
        code = channel->kickClient(_client, *client_to_kick, _msg);
    }
    if (code.code != SUCCESS.code) {
        std::string target;
        if (code.code == ERR_NOSUCHNICK.code) {
            target = _client_to_kick;
        }
        else {
            target = _channel;
        }
        msg.codedMessage(_client, _state, code, target);
    }
    else {
        std::string target = _channel + " " + _client_to_kick;
        msg.message(_client, _client, _command, target, _msg);
    }
}