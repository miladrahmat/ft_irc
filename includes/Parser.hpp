#pragma once
#include "Server.hpp"
#include "Client.hpp"
#include "JoinCommand.hpp"
#include <vector>

class	Parser {
	public:
	Parser() = default;
	~Parser() = default;
	void	parseCap(Client& client, std::string& input);
	void	parseCommand(Client& client, std::string& input);
	bool	parseJoinCommand(Client& client, std::string& input);
};