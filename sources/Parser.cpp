#include "Parser.hpp"

void Parser::parseRegisteration(std::shared_ptr<Client>& client, std::string& input, State& state) {
	try {
		std::string whitespace = " \t\r\n\t\v";
		std::string command = input.substr(0, input.find_first_of(whitespace));
		input.erase(0, command.length() + 1);
		input.erase(0, input.find_first_not_of(whitespace));
		if (command.compare("CAP") == 0) {
			return ;
		}
		else if (command.compare("PASS") == 0) {
			client->setPassword(input);
		} else if (command.compare("NICK") == 0) {
			std::unique_ptr<ACommand> cmd = parseNickCommand(client, command, input, state);
			if (cmd != nullptr)
				cmd->execute();
		} else if (command.compare("USER") == 0) {
			try {
				short int args = 1;
				for (size_t i = 0; i < input.length(); ++i) {
					if (input[i] == ':') {
						break ;
					}
					if (whitespace.find(input[i]) != std::string::npos) {
						i = input.find_first_not_of(whitespace, i) - 1;
						args++;
					}
				}
				if (args != 4) {
					throw std::runtime_error("Not enough parameters");
				}
				std::string	real_name;
				std::string	username = input.substr(0, input.find_first_of(' '));
				if (username.empty()) {
					throw std::runtime_error("Not enough parameters");
				}
				for (size_t i = 0; i < username.length(); i++) {
					std::string	invalid = " ,*?!@.:";
					if (invalid.find(username[i]) != std::string::npos) {
						throw std::runtime_error("Erroneus username");
					}
				}
				input.erase(0, username.length() + 1);
				if (input.find(':') == std::string::npos) {
					throw std::runtime_error("Not enough parameters");
				}
				
				else if (input.back() != ':') {
					real_name = input.substr(input.find(':') + 1, input.length());
				}
				client->setName(real_name);
				if (username[0] != '~') {
					client->setUsername("~" + username);
				}
				else {
					client->setUsername(username);
				}
			} catch (std::exception& e) {
				Message msg;
				std::string err(e.what());
				if (err == "Not enough parameters") {
					msg.codedMessage(client, state, ERR_NEEDMOREPARAMS, "USER");
				}
				else {
					struct reply repl = ERROR;
					repl.msg = err;
					msg.codedMessage(client, state, repl, "USER");
				}
			}
		}
	} catch (std::exception& e) {
		std::cerr << "In parser: " << e.what() << std::endl;
	}
}

std::unique_ptr<ACommand> Parser::parseCommand(std::shared_ptr<Client>& client, std::string& input,
	State& state) {

	std::string command;

	try {
		std::string whitespace = " \t\r\n\t\v";
		input.erase(0, input.find_first_not_of(whitespace));
		command = input.substr(0, input.find_first_of(whitespace));
		input.erase(0, command.length() + 1);
		for (size_t i = 0; i < input.length(); ++i) {
			if (input[i] == ':') {
				break ;
			}
			if (whitespace.find(input[i]) != std::string::npos) {
				input.erase(i, input.find_first_not_of(whitespace, i) - i);
				input.insert(i, 1, ' ');
				std::cout << input << std::endl;
			}
			if (i > input.length()) {
				break ;
			}
		}
		input.erase(0, input.find_first_not_of(whitespace));
		if (command.compare("JOIN") == 0) {
			return (parseJoinCommand(client, command, input, state));
		}
		else if (command.compare("PRIVMSG") == 0) {
			return (parsePrivmsgCommand(client, command, input, state));
		}
		else if (command.compare("MODE") == 0) {
			return (parseModeCommand(client, command, input, state));
		}
		else if (command.compare("QUIT") == 0) {
			return (parseQuitCommand(client, command, input, state));
		}
		else if (command.compare("NICK") == 0) {
			return (parseNickCommand(client, command, input, state));
		}
		else if (command.compare("KICK") == 0) {
			return (parseKickCommmand(client, command, input, state));
		}
		else if (command.compare("INVITE") == 0) {
			return (parseInviteCommand(client, command, input, state));
		}
		else if (command.compare("TOPIC") == 0) {
			return (parseTopicCommand(client, command, input, state));
		}
		else if (command.compare("WHO") == 0) {
			return (parseWhoCommand(client, command, input, state));
		}
		else if (command.compare("WHOIS") == 0) {
			return (parseWhoisCommand(client, command, input, state));
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	Message msg;
	msg.codedMessage(client, state, ERR_UNKNOWNCOMMAND, command);
	return (nullptr);
}

std::unique_ptr<ACommand> Parser::parseWhoCommand(std::shared_ptr<Client>& client, std::string&command, std::string& input, State& state) {
	std::string arg = input;
	return (WhoCommand::create(command, client, state, arg));
}

std::unique_ptr<ACommand> Parser::parseTopicCommand(std::shared_ptr<Client>& client,
	std::string& command, std::string& input, State& state) {

	std::string channel = "";
	if (!input.empty()) {
		channel = input.substr(0, input.find_first_of(' '));
		input.erase(0, channel.length() + 1);
	}
	return (TopicCommand::create(command, client, state, channel, input));
}

std::unique_ptr<ACommand> Parser::parseQuitCommand(std::shared_ptr<Client>& client,
	std::string& command, std::string& input, State& state) {

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
	std::string& command, std::string& input, State& state) {

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
	std::string& command, std::string& input, State& state) {

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
	std::string& command, std::string& input, State& state) {

	if (input[0] == ':') {
		input.erase(0, 1);
	}
	size_t pos = input.find_first_not_of(" \t");
	input.erase(0, pos);
	std::string nickname = input.substr(0, input.find_first_of(' '));
	return (NickCommand::create(command, client, state, nickname));
}

std::unique_ptr<ACommand> Parser::parseJoinCommand(std::shared_ptr<Client>& client,
	std::string& command, std::string& input, State& state) {

	std::vector<std::string> arg_vec;
	while (!input.empty()) {
		std::string	arg = input.substr(0, input.find_first_of(' '));
		input.erase(0, arg.length() + 1);
		arg_vec.push_back(arg);
	}
	return (JoinCommand::create(command, client, state, arg_vec));
}

std::unique_ptr<ACommand> Parser::parsePrivmsgCommand(std::shared_ptr<Client>& client,
	std::string& command, std::string& input, State& state) {

	std::string	target = input.substr(0, input.find_first_of(' '));
	input.erase(0, target.length() + 1);
	return (PrivmsgCommand::create(command, client, state, target, input));
}

std::unique_ptr<ACommand> Parser::parseModeCommand(std::shared_ptr<Client>& client,
	std::string& command, std::string& input, State& state) {

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
	std::string& command, std::string& input, State& state) {

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
