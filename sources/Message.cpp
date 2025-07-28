
#include "Message.hpp"

Message::Message() : _type(-1) {}

Message::~Message(){}

std::string & Message::getMsg() {
	return (_msg);
}

int	Message::getType() const {
	return (_type);
}

void Message::clearMsg() {
	_msg.clear();
}

void Message::clearSendMsg() {
	_send_msg.clear();
}

void Message::determineType(std::shared_ptr<Client>& client) {
	if (_msg.compare(0, 4, "NICK") == 0 && client->getNickname().empty() && _type != CAP_LS) {
		_send_msg = "CAP * ACK :multi-prefix\r\n";
		_type = CAP_REQ_AGAIN;
	}
	else if (_msg.compare(0, 6, "CAP LS") == 0 || _msg.compare(0, 4, "PASS") == 0 \
		|| _msg.compare(0, 4, "USER") == 0 || (_msg.compare(0, 4, "NICK") == 0 && _type == CAP_LS)) {
		if (_msg.compare(0, 6, "CAP LS") == 0) {
			_send_msg = "CAP * LS :multi-prefix account-notify account-tag invite-notify\r\n";
		}
		_type = CAP_LS;
	}
	else if (_msg.compare(0, 7, "CAP REQ") == 0) {
		if (!client->getNickname().empty()) {
			_send_msg = "CAP " + client->getNickname() + " ACK :multi-prefix\r\n";
		}
		else {
			_send_msg = "CAP * ACK :multi-prefix\r\n";
		}
		_type = CAP_REQ;
	}
	else if (_msg.compare(0, 7, "CAP END") == 0) {
		_type = CAP_END;
	}
	else if (_msg.compare(0, 4, "JOIN") == 0 || _msg.compare(0, 7, "PRIVMSG") == 0 \
		|| _msg.compare(0, 4, "NICK") == 0 || _msg.compare(0, 4, "QUIT") == 0 \
		|| _msg.compare(0, 4, "KICK") == 0 || _msg.compare(0, 6, "INVITE") == 0 \
		|| _msg.compare(0,4, "MODE") == 0 || _msg.compare(0, 5, "TOPIC") == 0 \
		|| _msg.compare(0,5, "WHOIS") == 0 ) {
		_type = CMD;
	}
	else if (_msg.compare(0, 4, "PING") == 0) {
		_type = PING;
	}
}

bool Message::getNextMessage(std::shared_ptr<Client>& client) {
	if (client.get() == nullptr) {
		return (false);
	}
	size_t pos = client->getBuffer().find("\r\n");
	if (pos != std::string::npos) {
		_msg = client->getBuffer().substr(0, pos);
		client->emptyBuffer(0, pos + 2);
		return (true);
	}
	return (false);
}

void Message::messageCap(std::shared_ptr<Client>& client) {
	if (_send_msg.empty()) {
		return ;
	}
	client->appendSendBuffer(_send_msg);
	_send_msg.clear();
}

void Message::messagePong(std::shared_ptr<Client>& client, std::string sender, std::string command,
	std::string target, std::string msg) {
	_send_msg = ":" + sender + " " + command + " " + target + " :" + msg + "\r\n";
	client->appendSendBuffer(_send_msg);
	_send_msg.clear();
}

void Message::welcomeMessage(std::shared_ptr<Client>& client, State& state) {
	reply replies[5] = {
		RPL_WELCOME, 
		RPL_YOURHOST, 
		RPL_CREATED, 
		RPL_MYINFO, 
		RPL_ISUPPORT
	};
	for (short int i = 0; i < 5; i++) {
		codedMessage(client, state, replies[i], {});
	}
}

void Message::codedMessage(std::shared_ptr<Client>& client, State& state, reply code,
	const std::optional<std::string>& target) {
	_send_msg = ":" + state.getServerName() + " " + code.code;
	if (client->getNickname().empty() && code.code != "ERROR") {
		_send_msg += " *";
	}
	else if (code.code != "ERROR") {
		_send_msg += " " + client->getNickname();
	}
	if (target) {
		_send_msg += " " + *target;
	}
	if (code.code == RPL_MYINFO.code || code.code == RPL_ISUPPORT.code) {
		_send_msg += " " + code.msg + "\r\n";
	}
	else if (!code.msg.empty()) {
		_send_msg += " :" + code.msg + "\r\n";
	}
	else {
		_send_msg += "\r\n";
	}
	client->appendSendBuffer(_send_msg);
	_send_msg.clear();
}

void Message::message(const std::shared_ptr<Client>& s_client, std::shared_ptr<Client> & r_client,
	std::string cmd, const std::optional<std::string>& target, const std::optional<std::string>& msg) {
	//:nickname!username@hostname COMMAND #channel : <message or description of event>
	_send_msg = ":" + s_client->getNickname() + "!" + s_client->getUsername() + "@" + s_client->getHostname() + " " + cmd;
	if (target) {
		_send_msg += " " + *target;
	}
	if (msg) {
		_send_msg += " :" + *msg;
	}
	_send_msg += "\r\n";
	r_client->appendSendBuffer(_send_msg);
	_send_msg.clear();
}