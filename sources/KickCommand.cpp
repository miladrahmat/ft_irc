#include "KickCommand.hpp"

KickCommand::KickCommand(std::string command, std::shared_ptr<Client>& client, State& state) : ACommand(command, client, state) {}
#include <iostream>
std::unique_ptr<ACommand>	KickCommand::create(std::string command, std::shared_ptr<Client>& client, State& state,
	std::vector<std::string> args) {
	KickCommand*	cmd = new KickCommand(command, client, state);
    //args[0] == channel name 
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

#include <iostream>
bool    KickCommand::execute() const {
    Message msg;
    Channel & channel = *_state.getChannel(_channel);
    std::cout << "In execution" << std::endl;
    std::shared_ptr<Client>  & client_to_kick = *_state.getClient(_client_to_kick);
    std::cout << "msg: " << _msg << std::endl;
    if (!channel.kickClient(_client, client_to_kick, _msg)) {
        //#haha You're not a channel operator
        msg.codedMessage(_client, ERR_CHANOPRIVSNEEDED, _channel);
    }
    std::string target = _channel + " " + _client_to_kick;
    msg.message(_client, _client, "KICK", target, _msg);
    //return false if kickin fails?
    //send kicked message to all
    return (true);
}