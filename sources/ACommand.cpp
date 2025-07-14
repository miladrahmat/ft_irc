
#include "ACommand.hpp"

ACommand::ACommand(std::string command, std::shared_ptr<Client> & client, State & state) :
    _command(command), _client(client), _state(state) {};

std::string    ACommand::getCommand() const {
    return (_command);
}