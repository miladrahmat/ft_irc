
#include "JoinCommand.hpp"

JoinCommand::JoinCommand(std::string command, Client & client) : ACommand(command, client) {}

bool ACommand::execute() const {

}

std::optional<JoinCommand> ACommand::create(std::string command, Client& client) {
    // if (command.empty()) return std::nullopt;
    // return JoinCommand(command, client);
    JoinCommand cmd(command, client);
}
