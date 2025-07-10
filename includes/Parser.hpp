#pragma once
#include "Server.hpp"
#include "Client.hpp"
#include "JoinCommand.hpp"
#include "PrivmsgCommand.hpp"
#include "QuitCommand.hpp"
#include "NickCommand.hpp"
#include "KickCommand.hpp"
#include "InviteCommand.hpp"
#include "TopicCommand.hpp"
#include <vector>
#include <memory>

class	Parser {
	public:
	Parser() = default;
	~Parser() = default;
	void	parseCap(std::shared_ptr<Client>& client, std::string& input, State& state);
	void	parseCommand(std::shared_ptr<Client>& client, std::string& input, State& state);
	void	parseNickCommand(std::shared_ptr<Client>& client, std::string& input, State& state);
	void	parseJoinCommand(std::shared_ptr<Client>& client, std::string& input, State& state);
	void	parsePrivmsgCommand(std::shared_ptr<Client>& client, std::string& input, State& state);
	void	parseQuitCommand(std::shared_ptr<Client>& client, std::string& input, State& state);
	void	parseKickCommmand(std::shared_ptr<Client>& client, std::string& input, State& state);
	void	parseInviteCommand(std::shared_ptr<Client>& client, std::string& input, State& state);
	void	parseTopicCommand(std::shared_ptr<Client>& client, std::string& input, State& state);
};