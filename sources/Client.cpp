#include "Client.hpp"
#include <iostream>

Client::Client(int socket) : _client_socket(socket) {
	_nickname = "hpirkola";
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

void	Client::setClientSocket(int socket) {
	_client_socket = socket;
}

void	Client::appendBuffer(std::string& msg) {
	_buffer.append(msg);
}

bool	Client::receiveData() {
	char	buf[1024];
	int		received_bytes = recv(this->_client_socket, buf, sizeof(buf), MSG_DONTWAIT);
	if (received_bytes > 0) {
		std::string msg(buf);
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

void	Client::handleMessage(std::string msg)
{
	if (msg.substr(0,6) == "CAP LS") {
		//-> send CAP * LS :
		_send_buffer.append("CAP * LS :\r\n");
		sendData();
	}
	if (msg.substr(0, 7) == "CAP REQ") {
		//-> send CAP * ACK:multi-prefix
		_send_buffer.append("CAP * ACK:multi-prefix :\r\n");
	}
	if (msg.substr(0, 7) == "CAP END") {
		std::cout << "Hello! Sending welcomes" << std::endl;
		_send_buffer.append(":ircserv 001" + _nickname + " :Welcome to IRC\r\n");
		sendData();
		_send_buffer.append(":ircserv 002" + _nickname + " :Your host is ircserv\r\n");
		sendData();
		_send_buffer.append(":ircserv 003" + _nickname + " :This server was created today\r\n");
		sendData();
		_send_buffer.append(":ircserv 004" + _nickname + " :ircserv 1.0\r\n");
		sendData();
	}
	else {
		//parse and send message or do actions based on it
	}
}