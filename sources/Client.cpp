#include "Client.hpp"
#include <iostream>

Client::Client(int socket) : _client_socket(socket) {
}

Client::Client(Client&& old_client) : _client_socket(old_client._client_socket), _name(old_client._name), _nickname(old_client._nickname) {
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

void	Client::setNickname(std::string nickname) {
	_nickname = nickname;
}

void	Client::setUsername(std::string username) {
	_username = username;
}

void	Client::setName(std::string name) {
	_name = name;
}

void	Client::setClientSocket(int socket) {
	_client_socket = socket;
}

void	Client::appendBuffer(std::string& msg) {
	_buffer.append(msg);
}

void	Client::appendSendBuffer(std::string& msg) {
	_send_buffer.append(msg);
}

bool	Client::receiveData() {
	char	buf[1024];
	int		received_bytes = recv(this->_client_socket, buf, sizeof(buf), MSG_DONTWAIT);
	if (received_bytes > 0) {
		std::string msg(buf, received_bytes);
		appendBuffer(msg);
		return (true);
	} else if (received_bytes == 0 || (received_bytes < 0 && errno != EWOULDBLOCK && errno != EAGAIN)) {
		return (false);
	}
	return (true);
}

bool	Client::sendData() {
	if (this->_send_buffer.empty()) { return (true); }
	int	sent_bytes = send(this->_client_socket, this->_send_buffer.c_str(), this->_send_buffer.size(), MSG_DONTWAIT);
	if (sent_bytes < 0 && errno != EWOULDBLOCK && errno != EAGAIN) { return (false); }
	this->_send_buffer.erase(0, sent_bytes);
	return (true);
}

std::string	Client::getSendBuffer() {
	return (_send_buffer);
}

std::string	Client::getBuffer() {
	return (_buffer);
}

void	Client::emptyBuffer(int begin, int end) {
	_buffer.erase(begin, end);
}