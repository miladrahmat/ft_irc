
#include "Channel.hpp"

Channel::Channel(std::string name, std::shared_ptr<Client> client, std::string password) :
   _name(name), _password(password), _invite_only(false), _topic_command_access(true),
    _user_limit(-1), clients{client}, operators{client}, topic("") {};

Channel::~Channel() {};


/****************************************************************************************
    GETTERS
****************************************************************************************/

std::string Channel::getName() const {
    return (_name);
}

bool Channel::getTopicMode() const {
    return (_topic_command_access);
}

int Channel::getUserLimit() {
    return (_user_limit);
}

std::string Channel::getPassword() const {
    return (_password);
}

int Channel::getSize() {
    return (clients.size());
}

std::vector<std::shared_ptr<Client>>::iterator Channel::getClient(std::string nickname) {
	std::vector<std::shared_ptr<Client>>::iterator it = clients.begin();
	for ( ; it != clients.end(); it++) {
		if ((*it)->getNickname() == nickname)
			return (it);
	}
	return (it);
}

std::string Channel::getModes(const std::shared_ptr<Client> & client) {
    std::string modes = "+";
    std::string params;
    if (_invite_only) {
        modes += "i";
    }
    if (_topic_command_access) {
        modes += "t";
    }
    if (_password != "") {
        modes += "k";
        params += _password;
    }
    if (_user_limit != -1) {
        modes += "l";
        if (params == "") {
            params = std::to_string(_user_limit);
        }
        else {
            params += " " + std::to_string(_user_limit);
        }
    }
    if (modes == "+") {
        return ("");
    }
    if (isClient(client)) {
        return (modes + " " + params);
    }
    else {
        return (modes);
    }
}

/****************************************************************************************
    CHECKS
****************************************************************************************/

bool Channel::isClient(const std::shared_ptr<Client> & client) const {
    if (std::find(clients.begin(), clients.end(), client) == clients.end()) {
        return (false);
    }
    return (true);
};

bool Channel::isOperator(const std::shared_ptr<Client> & client) const {
    if (std::find(operators.begin(), operators.end(), client) == operators.end()) {
        return (false);
    }
    return (true);
};

reply Channel::checkPrivileges(const std::shared_ptr<Client> & client) const {
    if (isOperator(client) == false) {
        return (ERR_CHANOPRIVSNEEDED);
    }
    return (SUCCESS);
}

bool Channel::channelFull(const std::shared_ptr<Client> & client) const {
    if (_user_limit == -1) {
        return (false);
    }
    if (clients.size() >= static_cast<std::size_t>(_user_limit) && !client->isInvited(this->_name)) {
        return (true);
    }
    return (false);
}

/****************************************************************************************
    OPERTIONAL FUNCTIONS
****************************************************************************************/

void Channel::setTopic(const std::shared_ptr<Client> & client, std::string new_topic) {
    topic = new_topic;
    topic_who = client;
    topic_when = std::time(0);
}

reply Channel::kickClient(const std::shared_ptr<Client> & client, const std::shared_ptr<Client> & client_to_kick, std::string msg) {
    if (isOperator(client)) {
        if (isClient(client_to_kick)) {
            std::string target = this->getName();
            target += " " + client_to_kick->getNickname();
            this->sendMsgToAll(client, "KICK", target, msg);
            this->removeOperator(client, client_to_kick);
            clients.erase(std::find(clients.begin(), clients.end(), client_to_kick));
            return (SUCCESS);
        }
        return (ERR_USERNOTINCHANNEL);
    }
    else {
        return (ERR_CHANOPRIVSNEEDED);
    }
}

reply Channel::inviteClient(const std::shared_ptr<Client> & client, std::shared_ptr<Client> & new_client) {
    Message msg;
    std::string target;

    if (!isClient(client)) {
        return (ERR_NOTONCHANNEL);
    }
    if (isClient(new_client)) {
        return  (ERR_USERONCHANNEL);
    }
    if (_invite_only && !isOperator(client)) {
        return (ERR_CHANOPRIVSNEEDED);
    }
    new_client->setInvitedTo(this->_name);
    return (RPL_INVITING);
}

reply Channel::setInviteMode(std::shared_ptr<Client> & client, bool set_to) {
    reply reply = checkPrivileges(client);
    if (reply.code == SUCCESS.code) {
        _invite_only = set_to;
    }
    return (reply);
}

reply Channel::setTopicMode(const std::shared_ptr<Client> & client, bool set_to) {
    reply reply = checkPrivileges(client);
    if (reply.code == SUCCESS.code) {
        _topic_command_access = set_to;
    }
    return (reply);
}

reply Channel::setChannelPassword(const std::shared_ptr<Client> & client, std::string new_password) {
    reply reply = checkPrivileges(client);
    if (reply.code == SUCCESS.code) {
        _password = new_password;
    }
    return (reply);
}

reply Channel::addOperator(const std::shared_ptr<Client> & client, const std::shared_ptr<Client> & new_operator) {
    reply reply = checkPrivileges(client);
    if (reply.code == SUCCESS.code) {
        operators.push_back(new_operator);
    }
    return (reply);
}

reply Channel::removeOperator(const std::shared_ptr<Client> & client, const std::shared_ptr<Client> & operator_to_remove) {
    reply reply = checkPrivileges(client);
    if (reply.code == SUCCESS.code) {
        if (isOperator(operator_to_remove)) {
            operators.erase(std::find(operators.begin(), operators.end(), operator_to_remove));
        }
    }
    return (reply);
}

reply Channel::setUserLimit(const std::shared_ptr<Client> & client, unsigned int limit) {
    reply reply = checkPrivileges(client);
    if (reply.code == SUCCESS.code) {
        _user_limit = limit;
    }
    return (reply);
}

reply Channel::join(const std::shared_ptr<Client> & client, std::string password) {
    if (_invite_only && !client->isInvited(this->_name)) {
        return (ERR_INVITEONLYCHAN);
    }
    if (channelFull(client)) {
        return (ERR_CHANNELISFULL);
    }
    if (_password != "" && password != _password) {
        return (ERR_BADCHANNELKEY);
    }
    if (client->getChannelCount() == CHAN_MAX) {
        return (ERR_TOOMANYCHANNELS);
    }
    clients.push_back(client);
    return (SUCCESS);
}

void    Channel::removeClient(const std::shared_ptr<Client> & client) {
    for (auto it = clients.begin(); it != clients.end(); it++) {
        if ((*it)->getClientSocket() == client->getClientSocket()) {
            clients.erase(it);
            break;
        }
    }
}

void    Channel::sendMsgToAll(const std::shared_ptr<Client>& client, std::string cmd, const std::optional<std::string>& target, const std::optional<std::string>& msg) {
    Message	message;

	for (auto it = this->clients.begin(); it != this->clients.end(); it++) {
		if (client != *it) {
			message.message(client, *it, cmd, target, msg);
        }
	}
}
