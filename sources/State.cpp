
#include "State.hpp"
#include <iostream>

int	State::getEpollFd() const {
	return (_epoll_fd);
}

std::vector<Channel>::iterator	State::getChannel(std::string channel_name) {
	std::vector<Channel>::iterator it = _channels.begin();
	std::cout << "Starting loop to find channels. Size: " << _channels.size() << std::endl;
	for ( ; it != _channels.end(); it++) {
		std::transform(channel_name.begin(), channel_name.end(), channel_name.begin(), ::tolower);
		std::cout << "HERE" << it->getName() << std::endl;
		if (it->getName() == channel_name) {
			return (it);
		}
	}
	return (it);
}

std::vector<Channel> & State::getChannels() {
	return (_channels);
}

void	State::addNewChannel(std::string name, std::shared_ptr<Client> & client, std::string password) {
	_channels.push_back(Channel(name, client, password));
	//some reply?
}

std::vector<std::shared_ptr<Client>>::iterator	State::getClient(std::string nickname) {
	std::vector<std::shared_ptr<Client>>::iterator it = _clients.begin();
	for ( ; it != _clients.end(); it++) {
		if ((*it)->getNickname() == nickname)
			return (it);
	}
	return (it);
}

std::vector<std::shared_ptr<Client>>&	State::getClients() {
	return (_clients);
}
