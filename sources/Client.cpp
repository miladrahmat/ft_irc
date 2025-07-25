#include "Client.hpp"

Client::Client(int socket, int epoll_fd, std::string ip) : _client_socket(socket), _epoll_fd(epoll_fd), _name(""), _nickname(""), _username(""), _hostname(ip), _password(""), _authenticated(false), _nick_validated(false), _pass_validated(true) {

}

Client::Client(Client&& old_client) noexcept : _client_socket(old_client._client_socket), _epoll_fd(old_client._epoll_fd), _name(old_client._name), _nickname(old_client._nickname), \
											_username(old_client._username), _hostname(old_client._hostname), _password(old_client._password), _buffer(old_client._buffer), \
											_send_buffer(old_client._send_buffer), _nick_validated(old_client._nick_validated), _pass_validated(old_client._pass_validated) {

}

Client::~Client() {
	close(_client_socket);
}

bool Client::operator==(const Client& other) const {
	return (this->_client_socket == other._client_socket);
}

int	Client::getClientSocket() const {
	return (_client_socket);
}

int	Client::getEpollFd() const {
	return (_epoll_fd);
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

bool Client::getNickValidated() const {
	return (_nick_validated);
}

void Client::validateNick() {
	_nick_validated = true;
}

bool Client::isAuthenticated() const {
	return (_authenticated);
}

bool Client::isValidPass() const {
	return (_pass_validated);
}

void Client::setNickname(std::string nickname) {
	_nickname = nickname;
}

void Client::setUsername(std::string username) {
	_username = username;
}

void Client::setHostname(std::string hostname) {
	_hostname = hostname;
}

void Client::setName(std::string name) {
	_name = name;
}

void Client::setPassword(std::string password) {
	_password = password;
}

void Client::setValidPass(bool set) {
	_pass_validated = set;
}

void Client::authenticate() {
	if (_name.empty() || _nickname.empty() || _hostname.empty() ||
		_username.empty() || _password.empty()) {
		return ;
	}
	_authenticated = true;
}

void Client::setClientSocket(int socket) {
	_client_socket = socket;
}

void Client::appendBuffer(std::string const& msg) {
	_buffer.append(msg);
}

void Client::appendSendBuffer(std::string const& msg) {
	_send_buffer.append(msg);
	changePut(EPOLLOUT);
}

void Client::changePut(uint32_t put) {
	struct epoll_event ev;
	ev.events = put;
	ev.data.fd = this->getClientSocket();
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, this->getClientSocket(), &ev) < 0) {
		_send_buffer.clear();
		throw std::runtime_error("epoll_ctl failed: " + std::string(strerror(errno)));
	}
}

bool Client::receiveData() {
	char buf[1024];
	int received_bytes = recv(this->_client_socket, buf, sizeof(buf), MSG_DONTWAIT);
	if (received_bytes > 0) {
		std::string msg(buf, received_bytes);
		appendBuffer(msg);
		return (true);
	}
	else if (received_bytes == 0 || (received_bytes < 0 && errno != EWOULDBLOCK && errno != EAGAIN)) {
		return (false);
	}
	return (true);
}

bool Client::sendData() {
	if (this->_send_buffer.empty()) {
		return (true);
	}
	int	sent_bytes = send(this->_client_socket, this->_send_buffer.c_str(), this->_send_buffer.size(), MSG_DONTWAIT);
	if (sent_bytes < 0 && errno != EWOULDBLOCK && errno != EAGAIN) {
		return (false);
	}
	this->_send_buffer.erase(0, sent_bytes);
	if (_send_buffer.empty()) {
		changePut(EPOLLIN);
	}
	return (true);
}

std::string	Client::getSendBuffer() {
	return (_send_buffer);
}

std::string	Client::getBuffer() {
	return (_buffer);
}

void Client::emptyBuffer(int begin, int end) {
	_buffer.erase(begin, end);
}

void Client::printClient() const {
	std::cout << "NAME: " << _name << std::endl;
	std::cout << "NICKNAME: " << _nickname << std::endl;
	std::cout << "HOSTNAME: " << _hostname << std::endl;
	std::cout << "USERNAME: " << _username << std::endl;
}

bool Client::isInvited(const std::string& channel_name) const {
	if (std::find(_invited_to.begin(), _invited_to.end(), channel_name) != _invited_to.end()) {
		return (true);
	}
	return (false);
}

void Client::setInvitedTo(std::string channel_name) {
	_invited_to.push_back(channel_name);
}