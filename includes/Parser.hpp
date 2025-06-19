#pragma once
#include "Server.hpp"
#include "Client.hpp"

class	Parser {
	public:
	Parser() = default;
	~Parser() = default;
	void	parseInput(Client& client, std::string& input);
	void	authenticateClient(Client& client, std::string& input);
	bool	validateNickname(std::string nickname);
};