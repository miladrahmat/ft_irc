#include "Message.hpp"

Message::Message(){}

Message::~Message(){}

std::string	Message::getMsg() {
	return (_msg);
}

void	Message::emptyMsg() {
	_msg.erase(0, _msg.size());
}

bool	Message::handleCap(Client& client) {
	if (_msg.substr(0,6) == "CAP LS") {
		_send_msg = "CAP * LS :\r\n";
		client.appendSendBuffer(_send_msg);
		_send_msg.erase(0, _send_msg.size());
		return (true);
	}
	if (_msg.substr(0, 7) == "CAP REQ") {
		_send_msg = "CAP * ACK:multi-prefix :\r\n";
		client.appendSendBuffer(_send_msg);
		_send_msg.erase(0, _send_msg.size());
		return (true);
	}
	if (_msg.substr(0, 7) == "CAP END") {
		for (int i = 0; i < 5; i++) {
			welcomeMessage(client, i);
			client.appendSendBuffer(_send_msg);
			_send_msg.erase(0, _send_msg.size());
		}
		return (true);
	}
	else {
		return (false);
	}
}

bool	Message::getNextMessage(Client& client) {
	size_t pos = client.getBuffer().find("\r\n");
	if (pos != std::string::npos) {
		_msg = client.getBuffer().substr(0, pos);
		client.emptyBuffer(0, pos + 2);
		return (true);
	}
	return (false);
}

void	Message::welcomeMessage(Client& client, int i) {
	reply replies[5] = {
		RPL_WELCOME, 
		RPL_YOURHOST, 
		RPL_CREATED, 
		RPL_MYINFO, 
		RPL_ISUPPORT
	};
	_send_msg = ":ircserv " + replies[i].code + " " + client.getNickname() + replies[i].msg;
}

void	Message::errorMessage(Client& client, reply err) {
	_send_msg = "ircserv " + err.code + " :" + err.msg;
	client.appendSendBuffer(_send_msg);
	_send_msg.erase(0, _send_msg.size());
}