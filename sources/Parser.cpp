#include "Parser.hpp"

void	Parser::parseCap(std::shared_ptr<Client>& client, std::string& input) {
	try {
		if (input.compare(0, 6, "CAP LS") == 0) {
			return ;
		}
		else if (input.compare(0, 4, "PASS") == 0) {
			std::string	password = input.substr(5, input.length());
			client->setPassword(password);
		} else if (input.compare(0, 4, "NICK") == 0) {
			std::string	nickname = input.substr(5, input.length());
			client->setNickname(nickname);
		} else if (input.compare(0, 4, "USER") == 0) {
			std::string	args = input.substr(5, input.length());
			std::string	username = args.substr(0, args.find(' '));
			args = args.substr(username.length() + 1, args.length());
			std::string	hostname = args.substr(0, args.find(' ')); // The hostname in the old protocol. Probably need to change.
			std::string	real_name = args.substr(args.find(':') + 1, args.length());
			client->setUsername(username);
			client->setHostname(hostname);
			client->setName(real_name);
		} 
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

void	Parser::parseCommand(std::shared_ptr<Client>& client, std::string& input, State& state) {
	try {
		if (input.compare(0, 4, "JOIN") == 0 || input.compare(0, 4, "join") == 0) {
			parseJoinCommand(client, input, state);
		}
		else if (input.compare(0, 7, "PRIVMSG") == 0) {
			parsePrivmsgCommand(client, input, state);
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

bool	Parser::parseJoinCommand(std::shared_ptr<Client>& client, std::string& input, State& state) {
	std::vector<std::string>	arg_vec;

	std::string	command = input.substr(0, input.find_first_of(' '));
	input.erase(0, command.length() + 1);
	while (!input.empty()) {
		std::string	arg = input.substr(0, input.find_first_of(' '));
		input.erase(0, arg.length() + 1);
		arg_vec.push_back(arg);
	}
	std::unique_ptr<ACommand>	cmd = JoinCommand::create(command, client, state, arg_vec);
	if (cmd == nullptr)
		return (false);
	return (cmd->execute());
}

bool	Parser::parsePrivmsgCommand(std::shared_ptr<Client>& client, std::string& input, State& state) {
	std::vector<std::string>	arg_vec;

	std::string command = input.substr(0, input.find_first_of(' '));
	input.erase(0, command.length() + 1);
	while (!input.empty()) {
		std::string	arg = input.substr(0, input.find_first_of(' '));
		input.erase(0, arg.length() + 1);
		arg_vec.push_back(arg);
	}
	std::unique_ptr<ACommand>	cmd = PrivmsgCommand::create(command, client, state, arg_vec);
	if (cmd == nullptr) {
		return (false);
	}
	return (cmd->execute());
}