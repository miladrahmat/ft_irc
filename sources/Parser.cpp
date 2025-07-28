#include "Parser.hpp"

void Parser::parseCap(std::shared_ptr<Client>& client, std::string& input, State& state) {
	try {
		if (input.compare(0, 6, "CAP LS") == 0) {
			return ;
		}
		else if (input.compare(0, 4, "PASS") == 0) {
			std::string	password = input.substr(5, input.length());
			client->setPassword(password);
		} else if (input.compare(0, 4, "NICK") == 0) {
			std::unique_ptr<ACommand> cmd = parseNickCommand(client, input, state);
			if (cmd != nullptr)
				cmd->execute();
		} else if (input.compare(0, 4, "USER") == 0) {
			std::string	args = input.substr(5, input.length());
			std::string	username = args.substr(0, args.find(' '));
			args = args.substr(username.length() + 1, args.length());
			std::string	real_name = args.substr(args.find(':') + 1, args.length());
			client->setUsername(username);
			client->setName(real_name);
		} 
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

std::unique_ptr<ACommand> Parser::parseCommand(std::shared_ptr<Client>& client, std::string& input,
	State& state) {

	try {
		if (input.compare(0, 4, "JOIN") == 0) {
			return (parseJoinCommand(client, input, state));
		}
		else if (input.compare(0, 7, "PRIVMSG") == 0) {
			return (parsePrivmsgCommand(client, input, state));
		}
		else if (input.compare(0, 4, "MODE") == 0) {
			return (parseModeCommand(client, input, state));
		}
		else if (input.compare(0, 4, "QUIT") == 0) {
			return (parseQuitCommand(client, input, state));
		}
		else if (input.compare(0, 4, "NICK") == 0) {
			return (parseNickCommand(client, input, state));
		}
		else if (input.compare(0, 4, "KICK") == 0) {
			return (parseKickCommmand(client, input, state));
		}
		else if (input.compare(0, 6, "INVITE") == 0) {
			return (parseInviteCommand(client, input, state));
		}
		else if (input.compare(0, 5, "TOPIC") == 0) {
			return (parseTopicCommand(client, input, state));
		}
		else if (input.compare(0,5, "WHOIS") == 0) {
			return (parseWhoisCommand(client, input, state));
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return (nullptr);
}

std::unique_ptr<ACommand> Parser::parseTopicCommand(std::shared_ptr<Client>& client,
	std::string& input, State& state) {

	std::string command = input.substr(0, input.find_first_of(' '));
	input.erase(0, command.length() + 1);
	std::string channel = "";
	if (!input.empty()) {
		channel = input.substr(0, input.find_first_of(' '));
		input.erase(0, channel.length() + 1);
	}
	std::string topic = "";
	if (!input.empty()) {
		if (input == ":") {
			topic = "";
		}
		else {
			topic = input.substr(1, input.length());
		}
	}
	return (TopicCommand::create(command, client, state, channel, topic));
}

std::unique_ptr<ACommand> Parser::parseQuitCommand(std::shared_ptr<Client>& client,
	std::string& input, State& state) {

	std::string command = input.substr(0, input.find_first_of(' '));
	input.erase(0, command.length() + 2);
	std::string arg;
	if (input != "leaving") {
		arg = input;
	}
	else {
		arg = "Client quit";
	}
	return (QuitCommand::create(command, client, state, arg));
}

std::unique_ptr<ACommand> Parser::parseKickCommmand(std::shared_ptr<Client>& client,
	std::string& input, State& state) {

	std::string command = input.substr(0, input.find_first_of(' '));
	input.erase(0, command.length() + 1);
	std::string channel = input.substr(0, input.find_first_of(' '));
	input.erase(0, channel.length() + 1);
	input.erase(0, input.find_first_not_of(" \t"));
	if (!input.empty() && input[0] == ':') {
		input.erase(0, 1);
	}
	std::string nick = input.substr(0, input.find_first_of(' '));
	input.erase(0, nick.length() + 1);
	if (!input.empty() && input[0] == ':') {
		input.erase(0, 1);
	}
	std::string message = input;

	return (KickCommand::create(command, client, state, channel, nick, message));
}

std::unique_ptr<ACommand> Parser::parseInviteCommand(std::shared_ptr<Client>& client,
	std::string& input, State& state) {

	std::string command = input.substr(0, input.find_first_of(' '));
	input.erase(0, command.length() + 1);
	input.erase(0, input.find_first_not_of(" \t"));
	if (!input.empty() && input[0] == ':') {
		input.erase(0, 1);
	}
	std::string nick = input.substr(0, input.find_first_of(' '));
	input.erase(0, nick.length() + 1);
	input.erase(0, input.find_first_not_of(" \t"));
	std::string channel = input;
	return (InviteCommand::create(command, client, state, nick, channel));
	
}

std::unique_ptr<ACommand> Parser::parseNickCommand(std::shared_ptr<Client>& client,
	std::string& input, State& state) {

	std::string command = input.substr(0, input.find_first_of(' '));
	input.erase(0, command.length() + 1);
	if (input[0] == ':') {
		input.erase(0, 1);
	}
	size_t pos = input.find_first_not_of(" \t");
	input.erase(0, pos);
	std::string nickname = input.substr(0, input.find_first_of(' '));
	return (NickCommand::create(command, client, state, nickname));
}

std::unique_ptr<ACommand> Parser::parseJoinCommand(std::shared_ptr<Client>& client,
	std::string& input, State& state) {

	std::vector<std::string> arg_vec;
	std::string	command = input.substr(0, input.find_first_of(' '));
	input.erase(0, command.length() + 1);
	while (!input.empty()) {
		std::string	arg = input.substr(0, input.find_first_of(' '));
		input.erase(0, arg.length() + 1);
		arg_vec.push_back(arg);
	}
	return (JoinCommand::create(command, client, state, arg_vec));
}

std::unique_ptr<ACommand> Parser::parsePrivmsgCommand(std::shared_ptr<Client>& client,
	std::string& input, State& state) {

	std::string command = input.substr(0, input.find_first_of(' '));
	input.erase(0, command.length() + 1);
	std::string	target = input.substr(0, input.find_first_of(' '));
	input.erase(0, target.length() + 1);
	return (PrivmsgCommand::create(command, client, state, target, input));
}

std::unique_ptr<ACommand> Parser::parseModeCommand(std::shared_ptr<Client>& client,
	std::string& input, State& state) {

	std::string command = input.substr(0, input.find_first_of(' '));
	input.erase(0, command.length() + 1);
	std::string target;
	if (!input.empty()) {
		target = input.substr(0, input.find_first_of(' '));
		input.erase(0, target.length() + 1);
	}
	std::string modes;
	if (!input.empty()) {
		modes = input.substr(0, input.find_first_of(' '));
		input.erase(0, modes.length() + 1);
	}
	std::string params;
	if (!input.empty()) {
		params = input;
	}
	return (ModeCommand::create(command, client, state, target, modes, params));
}

std::unique_ptr<ACommand> Parser::parseWhoisCommand(std::shared_ptr<Client>& client,
	std::string& input, State& state) {

	std::string command = input.substr(0, input.find_first_of(' '));
	input.erase(0, command.length() + 1);
	std::string server;
	if (!input.empty()) {
		server = input.substr(0, input.find_first_of(' '));
		input.erase(0, server.length() + 1);
	}
	std::string nick;
	if (!input.empty()) {
		nick = input.substr(0, input.find_first_of(' '));
		input.erase(0, nick.length() + 1);
	}
	else {
		nick = server;
		server = "";
	}
	return (WhoisCommand::create(command, client, state, server, nick));
}
