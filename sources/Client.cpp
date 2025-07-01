#include "Client.hpp"
#include <iostream>

Client::Client(int socket) : _client_socket(socket) {
	// std::vector<Channel&> _channels;
}

/* Client::Client(const Client & old_client) : _client_socket(old_client._client_socket), _name(old_client._name), _nickname(old_client._nickname), \
										_username(old_client._username), _hostname(old_client._hostname), _password(old_client._password), _buffer(old_client._buffer), _send_buffer(old_client._send_buffer) {
>>>>>>> 06940aac5c8c3c7c655454dbd1c6a76860ebd32d

} */

Client::Client(Client&& old_client) noexcept : _client_socket(old_client._client_socket), _name(old_client._name), _nickname(old_client._nickname), \
											_username(old_client._username), _hostname(old_client._hostname), _password(old_client._password), _buffer(old_client._buffer), _send_buffer(old_client._send_buffer) {

}

Client::~Client() {
	close(_client_socket);
}

/* Client		&Client::operator=(const Client& other) {
	if (this != &other) {
		this->_client_socket = other._client_socket;
		this->_name = other._name;
		this->_nickname = other._nickname;
		this->_username = other._username;
		this->_buffer = other._buffer;
		this->_send_buffer = other._buffer;
		this->_channels = other._channels;
	}
	return (*this);
} */

bool		Client::operator==(const Client& other) const {
	return (this->_client_socket == other._client_socket);
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

std::string	Client::getPassword() const {
	return (_password);
}

std::string	Client::getHostname() const {
	return (_hostname);
}

std::string	Client::getUsername() const {
	return(_username);
}

bool	Client::isAuthenticated() const {
	return (_authenticated);
}

void	Client::setNickname(std::string nickname) {
	_nickname = nickname;
}

bool	Client::validateNickname(std::string nickname) {
	std::string invalid_start = "$:#&~@+%";
	if (invalid_start.find(nickname[0]) != std::string::npos) {
		return (false);
	}
	std::string	invalid = " ,*?!@.";
	for (size_t i = 0; i < invalid.length(); i++) {
		if (nickname.find(invalid[i]) != std::string::npos) {
			return (false);
		}
	}
	setNickname(nickname);
	return (true);
}

void	Client::setUsername(std::string username) {
	_username = username;
}

void	Client::setHostname(std::string hostname) {
	_hostname = hostname;
}

void	Client::setName(std::string name) {
	_name = name;
}

void	Client::setPassword(std::string password) {
	_password = password;
}

void	Client::authenticate() {
	if (_name.empty())
		return ;
	if (_nickname.empty())
		return ;
	if (_hostname.empty())
		return ;
	if (_username.empty())
		return ;
	if (_password.empty())
		return ;
	_authenticated = true;
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

void	Client::printClient() const {
	std::cout << "NAME: " << _name << std::endl;
	std::cout << "NICKNAME: " << _nickname << std::endl;
	std::cout << "HOSTNAME: " << _hostname << std::endl;
	std::cout << "USERNAME: " << _username << std::endl;
}

std::string Client::getChannelInvitedTo() const {
	return (_invited_to);
}

void Client::setInvitedTo(std::string channel_name) {
	_invited_to = channel_name;
}


Channel* Client::getChannel(int i) const {
	return (_channels[i]);
}

int	Client::getChannelsSize() {
	return (_channels.size());
}