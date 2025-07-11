#include "KickCommand.hpp"

KickCommand::KickCommand(std::string command, std::shared_ptr<Client>& client, State& state) : ACommand(command, client, state) {}

std::unique_ptr<ACommand>	KickCommand::create(std::string command, std::shared_ptr<Client>& client, State& state,
	std::vector<std::string> args) {
	KickCommand*	cmd = new KickCommand(command, client, state);
    cmd->_channel = args[0];
    cmd->_client_to_kick = args[1];
    for (size_t i = 0; i < args.size(); i++) {
        if (i < 2)
            continue;
        if (i >= 3)
            cmd->_msg += " ";
        cmd->_msg += args[i];
    }
    return (std::unique_ptr<KickCommand>(cmd));
}


void    KickCommand::execute() const {
    Message msg;
    std::vector<Channel>::iterator channel = _state.getChannel(_channel);
    std::vector<std::shared_ptr<Client>>::iterator client_to_kick = _state.getClient(_client_to_kick);
    const reply code = channel->kickClient(_client, *client_to_kick, _msg);
    if (code.code != SUCCESS.code) {
        std::string target;
        if (code.code == ERR_NOSUCHNICK.code)
            target = _client_to_kick;
        else
            target = _channel;
        msg.codedMessage(_client, _state, code, target);
    }
    else {
        std::string target = _channel + " " + _client_to_kick;
        msg.message(_client, _client, "KICK", target, _msg);
    }
}