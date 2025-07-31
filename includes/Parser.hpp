
#pragma once

#include "Server.hpp"
#include "Client.hpp"
#include "JoinCommand.hpp"
#include "PrivmsgCommand.hpp"
#include "ModeCommand.hpp"
#include "QuitCommand.hpp"
#include "NickCommand.hpp"
#include "KickCommand.hpp"
#include "InviteCommand.hpp"
#include "TopicCommand.hpp"
#include "WhoCommand.hpp"
#include "WhoisCommand.hpp"
#include <vector>
#include <memory>

class	Parser {
	public:
	Parser() = default;
	~Parser() = default;
	void parseRegisteration(std::shared_ptr<Client>& client, std::string& input, State& state);
	std::unique_ptr<ACommand> parseCommand(std::shared_ptr<Client>& client, std::string& input, State& state);
	std::unique_ptr<ACommand> parseNickCommand(std::shared_ptr<Client>& client, std::string& command, std::string& input, State& state);
	std::unique_ptr<ACommand> parseJoinCommand(std::shared_ptr<Client>& client, std::string& command, std::string& input, State& state);
	std::unique_ptr<ACommand> parsePrivmsgCommand(std::shared_ptr<Client>& client, std::string& command, std::string& input, State& state);
	std::unique_ptr<ACommand> parseQuitCommand(std::shared_ptr<Client>& client, std::string& command, std::string& input, State& state);
	std::unique_ptr<ACommand> parseKickCommmand(std::shared_ptr<Client>& client, std::string& command, std::string& input, State& state);
	std::unique_ptr<ACommand> parseInviteCommand(std::shared_ptr<Client>& client, std::string& command, std::string& input, State& state);
	std::unique_ptr<ACommand> parseTopicCommand(std::shared_ptr<Client>& client, std::string& command, std::string& input, State& state);
	std::unique_ptr<ACommand> parseModeCommand(std::shared_ptr<Client>& client, std::string& command, std::string& input, State& state);
	std::unique_ptr<ACommand> parseWhoCommand(std::shared_ptr<Client>& client, std::string& command, std::string& input, State& state);
	std::unique_ptr<ACommand> parseWhoisCommand(std::shared_ptr<Client>& client, std::string& command, std::string& input, State& state);
};