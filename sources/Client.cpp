#include "Client.hpp"
#include <iostream>

Client::Client(int socket) : _client_socket(socket) {

}

Client::Client(Client&& old_client) noexcept : _client_socket(std::move(old_client._client_socket)), _real_name(std::move(old_client._real_name)), _nickname(std::move(old_client._nickname)), \
										_authenticated(std::move(old_client._authenticated)), _user_name(std::move(old_client._user_name)), _host_name(std::move(old_client._host_name)), _buffer(std::move(old_client._buffer)) {
	old_client.setClientSocket(-1);
}

Client::Client(Client const& old_client) : _client_socket(old_client._client_socket), _real_name(old_client._real_name), _nickname(old_client._nickname), \
										_authenticated(old_client._authenticated), _user_name(old_client._user_name), _host_name(old_client._host_name), _buffer(old_client._buffer) {

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

std::string	Client::getUserName() const {
	return (_user_name);
}

std::string	Client::getHostName() const {
	return (_host_name);
}

std::string	Client::getBuffer() const {
	return (_buffer);
}

void	Client::setClientSocket(int socket) {
	_client_socket = socket;
}

void	Client::setUserName(std::string& user_name) {
	_user_name = user_name;
}

void	Client::setRealName(std::string& real_name) {
	_real_name = real_name;
}

void	Client::setNickName(std::string& nickname) {
	_nickname = nickname;
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