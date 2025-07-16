
#include "Channel.hpp"

Channel::Channel(std::string name, std::shared_ptr<Client> client, std::string password) :
   _name(name), _password(password), _invite_only(false), _topic_command_access(true),
    _user_limit(-1), clients{client}, operators{client}, topic("") {};

Channel::~Channel() {};

std::string Channel::getName() const {
    return (_name);
}

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
    if (clients.size() >= static_cast<std::size_t>(_user_limit) && client->getChannelInvitedTo() != this->_name) {
        return (true);
    }
    return (false);
}

void Channel::setTopic(const std::shared_ptr<Client> & client, std::string new_topic) {
    topic = new_topic;
    topic_who = client;
    topic_when = std::time(0);
    return ;
    //TODO how to handle if client is not operator or channel does not allow topic change
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
        return (ERR_NOSUCHNICK);
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

//OK
//set = true -> _invite_only = true, else set = false -> _invite_only = false
reply Channel::setInviteMode(std::shared_ptr<Client> & client, bool set) {
    reply reply = checkPrivileges(client);
    if (reply.code == SUCCESS.code) {
        _invite_only = set;
    }
    return (reply);
}

//OK
//set = true -> _topic_command_access = true, else set = false -> _topic_command_access = false
reply Channel::setTopicMode(const std::shared_ptr<Client> & client, bool set) {
    reply reply = checkPrivileges(client);
    if (reply.code == SUCCESS.code) {
        _topic_command_access = set;
    }
    return (reply);
}

bool Channel::getTopicMode() const {
    return (_topic_command_access);
}

//OK
//if '/MODE -k' then set new_password to ""
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

//OK
reply Channel::removeOperator(const std::shared_ptr<Client> & client, const std::shared_ptr<Client> & operator_to_remove) {
    reply reply = checkPrivileges(client);
    if (reply.code == SUCCESS.code) {
        if (isOperator(operator_to_remove)) {
            operators.erase(std::find(operators.begin(), operators.end(), operator_to_remove));
        }
    }
    return (reply);
}


//limit -1 means no limit
reply Channel::setUserLimit(const std::shared_ptr<Client> & client, unsigned int limit) {
    reply reply = checkPrivileges(client);
    if (reply.code == SUCCESS.code) {
        _user_limit = limit;
    }
    return (reply);
}

reply Channel::join(const std::shared_ptr<Client> & client, std::string password) {
    if (_invite_only && _name != client->getChannelInvitedTo()) {
        return (ERR_INVITEONLYCHAN);
    }
    if (channelFull(client)) {
        //TODO but not when invited????
        return (ERR_CHANNELISFULL);
    }
    if (_password != "" && password != _password) {
        return (ERR_BADCHANNELKEY);
    }
    clients.push_back(client);
    return (SUCCESS);
}

std::string Channel::getClientsNick() const {
    std::string nicks;
    for (auto i = clients.begin(); i != clients.end(); i++) {
        nicks.append((*i)->getName());
    }
    return (nicks);
}

void    Channel::removeClient(const std::shared_ptr<Client> & client) {
    for (auto it = clients.begin(); it != clients.end(); it++) {
        if ((*it)->getClientSocket() == client->getClientSocket()) {
            clients.erase(it);
            break;
        }
    }
}

int Channel::getSize() {
    return (clients.size());
}

void    Channel::sendMsgToAll(const std::shared_ptr<Client>& client, std::string cmd, const std::optional<std::string>& target, const std::optional<std::string>& msg) {
    Message	message;

	for (auto it = this->clients.begin(); it != this->clients.end(); it++) {
		if (client != *it) {
			message.message(client, *it, cmd, target, msg);
        }
	}
}

std::vector<std::shared_ptr<Client>>::iterator Channel::getClient(std::string nickname) {
	std::vector<std::shared_ptr<Client>>::iterator it = clients.begin();
	for ( ; it != clients.end(); it++) {
		if ((*it)->getNickname() == nickname)
			return (it);
	}
	return (it);
}

std::string Channel::getPassword() const {
    return (_password);
}

int Channel::getUserLimit() {
    return (_user_limit);
}

std::string Channel::getModes() {
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
    return (modes + " " + params);
}