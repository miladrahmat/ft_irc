#include "WhoCommand.hpp"

WhoCommand::WhoCommand(std::string command, std::shared_ptr<Client>& client, State& state) :
	ACommand(command, client, state) {}

std::unique_ptr<ACommand> WhoCommand::create(std::string command, std::shared_ptr<Client>& client,
	State& state, std::string arg) {
    
    WhoCommand* cmd = new WhoCommand(command, client, state);
    if (arg[0] == '#')
        cmd->_channel = arg;
    else
        cmd->_nick = arg;
    return (std::unique_ptr<ACommand>(cmd));
}

void WhoCommand::sendReply(const std::shared_ptr<Client>& client, const std::string& channel, const std::string& flags) const {
    struct reply whoReply = RPL_WHOREPLY;
    std::string target;

    whoReply.msg.append(client->getName());
    target += channel + " " + client->getUsername() + " " + client->getHostname() + " " + _state.getServerName() \
        + " " + client->getNickname() + " " + flags;
    
    Message msg;
    msg.codedMessage(_client, _state, whoReply, target);
}

void WhoCommand::execute() const {
    Message msg;
    std::string target;

    if (!_channel.empty()) {
        std::vector<Channel>::iterator channel = _state.getChannel(_channel);
        if (channel != _state.getChannels().end()) {
            for (auto it = channel->clients.begin(); it != channel->clients.end(); ++it) {
                std::string flags = "H";
                if (channel->isOperator(*it)) {
                    flags += "@";
                }
                sendReply(*it, _channel, flags);
            }
        }
        msg.codedMessage(_client, _state, RPL_ENDOFWHO, _channel);
    }
    else {
        std::vector<std::shared_ptr<Client>>::iterator client = _state.getClient(_nick);
        if (client != _state.getClients().end()) {
            std::string flags = "H";
            std::string shared_channel;
            for (auto it = _state.getChannels().begin(); it != _state.getChannels().end(); ++it) {
                if (it->isClient(*client) && it->isClient(_client)) {
                    shared_channel = it->getName();
                }
            }
            if (shared_channel.empty()) {
                shared_channel = "*";
            }
            else if (_state.getChannel(shared_channel)->isOperator(*client)) {
                flags += "@";
            }
            sendReply(*client, shared_channel, flags);
        }
        msg.codedMessage(_client, _state, RPL_ENDOFWHO, _nick);
    }
}