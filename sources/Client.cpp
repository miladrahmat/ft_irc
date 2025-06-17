#include "Client.hpp"
#include <iostream>

Client::Client(int socket) : _client_socket(socket) {
}

Client::Client(Client&& old_client) noexcept : _client_socket(std::move(old_client._client_socket)), _real_name(std::move(old_client._real_name)), _nickname(std::move(old_client._nickname)), \
										_authenticated(std::move(old_client._authenticated)), _username(std::move(old_client._username)), _host_name(std::move(old_client._host_name)), _buffer(std::move(old_client._buffer)) {
	old_client.setClientSocket(-1);
}

Client::Client(Client const& old_client) : _client_socket(old_client._client_socket), _real_name(old_client._real_name), _nickname(old_client._nickname), \
										_authenticated(old_client._authenticated), _username(old_client._username), _host_name(old_client._host_name), _buffer(old_client._buffer) {

}

Client::~Client() {
	close(_client_socket);
}

int	Client::getClientSocket() const {
	return (_client_socket);
}

bool	Client::isAuthenticated() const {
	return (_authenticated);
}

std::string	Client::getRealName() const {
	return (_real_name);
}

std::string	Client::getNickname() const {
	return (_nickname);
}

std::string	Client::getHostName() const {
	return (_host_name);
}

std::string	Client::getBuffer() const {
	return (_buffer);
}

void	Client::setNickname(std::string& nickname) {
	_nickname = nickname;
}

void	Client::setUsername(std::string& username) {
	_username = username;
}

void	Client::setClientSocket(int socket) {
	_client_socket = socket;
}

void	Client::setRealName(std::string& real_name) {
	_real_name = real_name;
}

void	Client::setHostName(std::string& host_name) {
	_host_name = host_name;
}

void	Client::authenticate() {
	_authenticated = true;
}

void	Client::appendBuffer(std::string& msg) {
	_buffer.append(msg);
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
	if (this->_buffer.empty()) { return (true); }
	int	sent_bytes = send(this->_client_socket, this->_buffer.c_str(), this->_buffer.size(), MSG_DONTWAIT);
	if (sent_bytes < 0 && errno != EWOULDBLOCK && errno != EAGAIN) { return (false); }
	this->_buffer.erase(0, sent_bytes);
	return (true);
}

bool	Client::getNextMessage(std::string &msg)
{
	size_t pos = this->_buffer.find("\r\n");
	if (pos != std::string::npos) {
		msg = this->_buffer.substr(0, pos);
		this->_buffer.erase(0, pos + 2);
		return (true);
	}
	return (false);
}
