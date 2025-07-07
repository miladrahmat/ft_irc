
#include "Channel.hpp"
#include <iostream>

Channel::Channel(std::string name, std::shared_ptr<Client> client, std::string password) :
   _name(name), _password(password), _invite_only(false), _topic_command_access(true),
    _user_limit(-1), clients{client},
    operators{client} {};

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

bool Channel::channelFull() const {
    if (_user_limit == -1) {
        return (false);
    }
    if (clients.size() == static_cast<std::size_t>(_user_limit)) {
        return (true);
    }
    return (false);
}

// void Channel::setTopic(const std::shared_ptr<Client> & client, std::string new_topic) {
//     if (isOperator(client) && _topic_command_access) {
//         _topic = new_topic;
//         return ;
//     }
//     //TODO how to handle if client is not operator or channel does not allow topic change
// }

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

bool Channel::inviteClient(const std::shared_ptr<Client> & client, std::shared_ptr<Client> & new_client) {
    if (!isClient(client)) {
        //ERR_NOTONCHANNEL (442);
        return (false);
    }
    if (isClient(new_client)) {
        //ERR_USERONCHANNEL (443)
        return  (false);
    }
    if (_invite_only && !isOperator(client)) {
        //ERR_CHANOPRIVSNEEDED (482)
        return (false);
    }
    new_client->setInvitedTo(this->_name);
    //RPL_INVITING (341)
    return (true);
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
    if (channelFull()) {
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