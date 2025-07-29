
#include "WhoisCommand.hpp"

WhoisCommand::WhoisCommand(std::string command, std::shared_ptr<Client>& client, State& state) :
    ACommand(command, client, state){}

std::unique_ptr<ACommand> WhoisCommand::create(std::string command, std::shared_ptr<Client>& client,
    State& state, std::string server, std::string nick) {
    
    WhoisCommand *cmd = new WhoisCommand(command, client, state);
    cmd->_nick = nick;
    cmd->_server = server;
    return (std::unique_ptr<WhoisCommand>(cmd));
}

void WhoisCommand::execute() const {
    Message msg;
    if (_server != "" && _server != _state.getServerName()) {
        msg.codedMessage(_client, _state, ERR_NOSUCHSERVER, _server);
        return ;
    }
    if (_nick == "") {
        msg.codedMessage(_client, _state, ERR_NONICKNAMEGIVEN, {});
        return ;
    }
    if (_state.getClient(_nick) == _state.getClients().end()) {
        msg.codedMessage(_client, _state, ERR_NOSUCHNICK, _nick);
        return ;
    }
    msg.codedMessage(_client, _state, RPL_ENDOFWHOIS, _nick);
}