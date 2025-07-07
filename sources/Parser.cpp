#include "Parser.hpp"

void	Parser::parseCap(std::shared_ptr<Client>& client, std::string& input, State& state) {
	try {
		if (input.compare(0, 6, "CAP LS") == 0) {
			return ;
		}
		else if (input.compare(0, 4, "PASS") == 0) {
			std::string	password = input.substr(5, input.length());
			client->setPassword(password);
		} else if (input.compare(0, 4, "NICK") == 0) {
			parseNickCommand(client, input, state);
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
		else if (input.compare(0, 4, "QUIT") == 0) {
			parseQuitCommand(client, input, state);
		}
		else if (input.compare(0, 4, "NICK") == 0) {
			parseNickCommand(client, input, state);
		}
		else if (input.compare(0, 4, "KICK") == 0) {
			parseKickCommmand(client, input, state);
		}
		else if (input.compare(0, 6, "INVITE") == 0) {
			parseInviteCommand(client, input, state);
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

void	Parser::parseQuitCommand(std::shared_ptr<Client>& client, std::string& input, State& state) {
	std::string command = input.substr(0, input.find_first_of(' '));
	input.erase(0, command.length() + 2);
	std::string arg;
	if (input != "leaving") {
		arg = input;
	}
	else {
		arg = "Client quit";
	}
	std::unique_ptr<ACommand>	cmd = QuitCommand::create(command, client, state, arg);
	if (cmd == nullptr)
		return ;
	cmd->execute();
}

void	Parser::parseKickCommmand(std::shared_ptr<Client>& client, std::string& input, State& state) {
	std::vector<std::string>	arg_vec;

	std::string command = input.substr(0, input.find_first_of(' '));
	input.erase(0, command.length() + 1);
	while (!input.empty()) {
		std::string	arg = input.substr(0, input.find_first_of(' '));
		input.erase(0, arg.length() + 1);
		arg_vec.push_back(arg);
	}
	std::unique_ptr<ACommand> cmd = KickCommand::create(command, client, state, arg_vec);
	if (cmd == nullptr)
		return ;
	cmd->execute();
}

void	Parser::parseInviteCommand(std::shared_ptr<Client>& client, std::string& input, State& state) {
	std::vector<std::string>	arg_vec;
	
	std::string command = input.substr(0, input.find_first_of(' '));
	input.erase(0, command.length() + 1);
	while (!input.empty()) {
		std::string	arg = input.substr(0, input.find_first_of(' '));
		input.erase(0, arg.length() + 1);
		if (!input.empty() && input[0] == ':')
			input.erase(0, 1);
		arg_vec.push_back(arg);
	}
	std::unique_ptr<ACommand> cmd = InviteCommand::create(command, client, state, arg_vec);
	if (cmd == nullptr)
		return ;
	cmd->execute();
}

void	Parser::parseNickCommand(std::shared_ptr<Client>& client, std::string& input, State& state) {
	std::string command = input.substr(0, input.find_first_of(' '));
	std::string nickname = input.substr(input.find_first_of(' ') + 1, input.length());
	std::unique_ptr<ACommand> cmd = NickCommand::create(command, client, state, nickname);

	cmd->execute();
}

void	Parser::parseJoinCommand(std::shared_ptr<Client>& client, std::string& input, State& state) {
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
		return ;
	cmd->execute();
}

void	Parser::parsePrivmsgCommand(std::shared_ptr<Client>& client, std::string& input, State& state) {
	std::string command = input.substr(0, input.find_first_of(' '));
	input.erase(0, command.length() + 1);
	std::string	target = input.substr(0, input.find_first_of(' '));
	input.erase(0, target.length() + 1);
	std::unique_ptr<ACommand>	cmd = PrivmsgCommand::create(command, client, state, target, input);
	if (cmd == nullptr) {
		return ;
	}
	cmd->execute();
}