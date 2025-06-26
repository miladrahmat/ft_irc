
#include "ACommand.hpp"

ACommand::ACommand(std::string command, Client & client, State & state) :
    _command(command), _client(client), _state(state) {};