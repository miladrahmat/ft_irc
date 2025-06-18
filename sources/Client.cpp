#include "Client.hpp"
#include <iostream>

Client::Client(int socket) : _client_socket(socket) {

}

Client::Client(const Client & old_client) noexcept : _client_socket(old_client._client_socket), _name(old_client._name), _nickname(old_client._nickname) {

}

Client::Client(Client&& old_client) noexcept : _client_socket(old_client._client_socket), _name(old_client._name), _nickname(old_client._nickname) {
	old_client.setClientSocket(-1);
}

Client::~Client() {
	close(_client_socket);
}

int	Client::getClientSocket() const {
	return (_client_socket);
}

std::string	Client::getName() const {
	return (_name);
}

std::string	Client::getNickname() const {
	return (_nickname);
}

void	Client::setClientSocket(int socket) {
	_client_socket = socket;
}