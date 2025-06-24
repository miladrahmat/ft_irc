
#include "ACommand.hpp"

ACommand::ACommand(std::string command, Client & client, Server & server) :
    _command(command), _client(client), _server(server) {};