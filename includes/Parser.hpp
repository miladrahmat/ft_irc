#pragma once
#include "Server.hpp"
#include "Client.hpp"

class	Parser {
	public:
	Parser() = default;
	~Parser() = default;
	void	parseCap(Client& client, std::string& input);
};