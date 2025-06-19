#include "Parser.hpp"

void	Parser::parseInput(Client& client, std::string& input) {
	if (input.substr(0, 4) == "NICK") {
		std::string	nickname = input.substr(5, input.length());
		if (validateNickname(nickname)) {
			client.setNickname(nickname);
		}
	} else if (input.substr(0, 4) == "USER") {
		std::string	args = input.substr(5, input.length());
		std::string	username = args.substr(0, args.find(' '));
		args = args.substr(username.length() + 1, args.length());
		std::string	hostname = args.substr(0, args.find(' ')); // The hostname in the old protocol. Probably need to change.
		std::string	real_name = args.substr(args.find(':') + 1, args.length());
		client.setUsername(username);
		client.setHostname(hostname);
		client.setName(real_name);
	} else if (input.substr(0, 4) == "PASS") {
		// TODO: How to get access to the password attribute found in Server class?

		/* std::string	password = input.substr(4, input.length());
		if (password == Server::getPassword()); */
	}
}

bool	Parser::validateNickname(std::string nickname) {
	std::string invalid_start = "$:#&~@+%";
	if (invalid_start.find(nickname[0]) != std::string::npos) {
		return (false);
	}
	std::string	invalid = " ,*?!@.";
	for (size_t i = 0; i < invalid.length(); i++) {
		if (nickname.find(invalid[i]) != std::string::npos) {
			return (false);
		}
	}
	return (true);
}