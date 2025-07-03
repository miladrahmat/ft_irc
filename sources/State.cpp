
#include "State.hpp"
#include <iostream>

std::vector<Channel>::iterator	State::getChannel(std::string channel_name) {
	std::vector<Channel>::iterator it = _channels.begin();
	for ( ; it != _channels.end(); it++) {
		std::transform(channel_name.begin(), channel_name.end(), channel_name.begin(), ::tolower);
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

void	State::removeClient(std::shared_ptr<Client>& client) {
	std::cout << "channels now: " << _channels.size() << std::endl;
	std::cout << "clients now: " << _clients.size() << std::endl;
	struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = client->getClientSocket();
	epoll_ctl(client->getEpollFd(), EPOLL_CTL_DEL, client->getClientSocket(), &ev);
	for (auto it = _channels.begin(); it != _channels.end();) {
		if (it->isClient(client)) {
			it->removeClient(client);
			if (it->getSize() == 0) {
				it = _channels.erase(it);
			}
			else {
				it->sendMsgToAll(client, "left the chat");
				it++;
			}
			//send message to all clients in the channel
		}
		else {
			it++;
		}
	}
	std::cout << "remaining channels: " << _channels.size() << std::endl;
	for (std::vector<std::shared_ptr<Client>>::size_type i = 0; i < _clients.size(); i++) {
		if (_clients[i]->getClientSocket() == client->getClientSocket()) {
			_clients[i]->getSendBuffer().clear();
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
	std::cout << "remaining clients: " << _clients.size() << std::endl;
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
