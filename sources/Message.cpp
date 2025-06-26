#include "Message.hpp"

Message::Message(){}

Message::~Message(){}

std::string&	Message::getMsg() {
	return (_msg);
}

int	Message::getType() const {
	return (_type);
}

void	Message::emptyMsg() {
	_msg.erase(0, _msg.size());
}

void	Message::handleCap(Client& client) {
	if (_msg.substr(0,6) == "CAP LS") {
		_send_msg = "CAP * LS :\r\n";
		client.appendSendBuffer(_send_msg);
		_send_msg.erase(0, _send_msg.size());
		_type = CAP_START;
	}
	else if (_msg.substr(0, 7) == "CAP REQ") {
		_send_msg = "CAP * ACK:multi-prefix :\r\n";
		client.appendSendBuffer(_send_msg);
		_send_msg.erase(0, _send_msg.size());
		_type = CAP_REQ;
	}
	else if (_msg.substr(0, 7) == "CAP END") {
		_type = CAP_END;
	}
	else if (_msg.compare(0, 4, "PASS") == 0 || _msg.compare(0, 4, "NICK") == 0 \
			|| _msg.compare(0, 4, "USER") == 0) {
		_type = CAP;
	}
	else if (_msg.compare(0, 4, "JOIN") == 0 || _msg.compare(0, 4, "join") == 0) {
		_type = CMD;
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

void	Message::welcomeMessage(Client& client) {
	reply replies[5] = {
		RPL_WELCOME, 
		RPL_YOURHOST, 
		RPL_CREATED, 
		RPL_MYINFO, 
		RPL_ISUPPORT
	};
	for (short int i = 0; i < 5; i++) {
		_send_msg = ":ircserv " + replies[i].code + " " + client.getNickname() + replies[i].msg;
		client.appendSendBuffer(_send_msg);
		_send_msg.clear();
	}
}

void	Message::errorMessage(Client& client, reply err) {
	_send_msg = "ircserv " + err.code + " :" + err.msg;
	client.appendSendBuffer(_send_msg);
	_send_msg.erase(0, _send_msg.size());
}