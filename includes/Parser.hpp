#pragma once
#include "Server.hpp"
#include "Client.hpp"
#include "JoinCommand.hpp"
#include "PrivmsgCommand.hpp"
#include <vector>
#include <memory>

class	Parser {
	public:
	Parser() = default;
	~Parser() = default;
	void	parseCap(std::shared_ptr<Client>& client, std::string& input);
	void	parseCommand(std::shared_ptr<Client>& client, std::string& input);
	bool	parseJoinCommand(std::shared_ptr<Client>& client, std::string& input);
	bool	parsePrivmsgCommand(std::shared_ptr<Client>& client, std::string& input);
};