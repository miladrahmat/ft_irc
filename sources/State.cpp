
#include "State.hpp"

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
	struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = client->getClientSocket();
	epoll_ctl(client.getEpollFd(), EPOLL_CTL_DEL, client->getClientSocket(), &ev);
	for (auto it = _channels.begin(); it != _channels.end();) {
		if (it->isClient(client)) {
			it->removeClient(client);
			if (it->getSize() == 0) {
				it = _channels.erase(it);
			}
			else {
				it++;
			}
			//send message to all clients in the channel
		}
		else {
			it++;
		}
	}
	for (std::vector<std::shared_ptr<Client>>::size_type i = 0; i < _state._clients.size(); i++) {
		if (_state._clients[i]->getClientSocket() == client->getClientSocket()) {
			_state._clients.erase(_state._clients.begin() + i);
			break;
		}
	}
}