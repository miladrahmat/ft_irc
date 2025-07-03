#pragma once
#include "Server.hpp"
#include "Client.hpp"
#include "JoinCommand.hpp"
#include "PrivmsgCommand.hpp"
#include "NickCommand.hpp"
#include <vector>
#include <memory>

class	Parser {
	public:
	Parser() = default;
	~Parser() = default;
	void	parseCap(std::shared_ptr<Client>& client, std::string& input, State& state);
	void	parseCommand(std::shared_ptr<Client>& client, std::string& input, State& state);
	bool	parseNickCommand(std::shared_ptr<Client>& client, std::string& input, State& state);
	bool	parseJoinCommand(std::shared_ptr<Client>& client, std::string& input, State& state);
	bool	parsePrivmsgCommand(std::shared_ptr<Client>& client, std::string& input, State& state);
};