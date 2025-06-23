#include "Parser.hpp"

void	Parser::parseCap(Client& client, std::string& input) {
	try {
		if (input.compare(0, 4, "PASS") == 0) {
			std::cout << "Setting password" << std::endl;
			std::string	password = input.substr(5, input.length());
			client.setPassword(password);
		} else if (input.compare(0, 4, "NICK") == 0) {
			std::cout << "Setting nickname" << std::endl;
			std::string	nickname = input.substr(5, input.length());
			client.setNickname(nickname);
		} else if (input.compare(0, 4, "USER") == 0) {
			std::cout << "Setting everything else" << std::endl;
			std::string	args = input.substr(5, input.length());
			std::string	username = args.substr(0, args.find(' '));
			args = args.substr(username.length() + 1, args.length());
			std::string	hostname = args.substr(0, args.find(' ')); // The hostname in the old protocol. Probably need to change.
			std::string	real_name = args.substr(args.find(':') + 1, args.length());
			client.setUsername(username);
			client.setHostname(hostname);
			client.setName(real_name);
		} 
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
