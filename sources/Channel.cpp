
#include "Channel.hpp"

Channel::Channel(std::string name, std::shared_ptr<Client> client, std::string password) :
   _name(name), _password(password), _invite_only(false), _topic_command_access(true),
    _user_limit(-1), _clients{client},
    _operators{client} {};

Channel::~Channel() {};

std::string Channel::getName() const {
    return (_name);
}

bool Channel::isClient(const std::shared_ptr<Client> & client) const {
    if (std::find(_clients.begin(), _clients.end(), client) == _clients.end()) {
        return (false);
    }
    return (true);
};

bool Channel::isOperator(const std::shared_ptr<Client> & client) const {
    if (std::find(_operators.begin(), _operators.end(), client) == _operators.end()) {
        return (false);
    }
    return (true);
};

bool Channel::channelFull() const {
    if (_user_limit == -1) {
        return (false);
    }
    if (_clients.size() == static_cast<std::size_t>(_user_limit)) {
        return (true);
    }
    return (false);
}

std::string Channel::showTopic() const {
    return (_topic);
}

void Channel::setTopic(const std::shared_ptr<Client> & client, std::string new_topic) {
    if (isOperator(client) && _topic_command_access) {
        _topic = new_topic;
        return ;
    }
    //TODO how to handle if client is not operator or channel does not allow topic change
}

reply Channel::kickClient(const std::shared_ptr<Client> & client, const std::shared_ptr<Client> & client_to_kick, std::string msg) {
    if (isOperator(client)) {
        if (isClient(client_to_kick)) {
            std::string target = this->getName();
            target += " " + client_to_kick->getNickname();
            this->sendMsgToAll(client, "KICK", target, msg);
            _clients.erase(std::find(_clients.begin(), _clients.end(), client_to_kick));
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

//set = true -> _invite_only = true, else set = false -> _invite_only = false
void Channel::setInviteMode(const std::shared_ptr<Client> & client, bool set) {
    if (isOperator(client)) {
        _invite_only = set;
    }
}

//set = true -> _topic_command_access = true, else set = false -> _topic_command_access = false
void Channel::setTopicMode(const std::shared_ptr<Client> & client, bool set) {
    if (isOperator(client)) {
        _topic_command_access = set;
    }
}

//if '/MODE -k' then set new_password to ""
void Channel::setChannelPassword(const std::shared_ptr<Client> & client, std::string new_password) {
    if (isOperator(client)) {
        //any rules for the password???
        _password = new_password;
    }
}

void Channel::addOperator(const std::shared_ptr<Client> & client, const std::shared_ptr<Client> & new_operator) {
    if (isOperator(client)) {
        _operators.push_back(new_operator);
    }
}

void Channel::removeOperator(const std::shared_ptr<Client> & client, const std::shared_ptr<Client> & operator_to_remove) {
    if (isOperator(client)) {
        if (isOperator(operator_to_remove)) {
            _operators.erase(std::find(_operators.begin(), _operators.end(), operator_to_remove));
        }
        //TODO operator_to_remove is not operator
    }
}

//limit -1 means no limit
void Channel::setUserLimit(const std::shared_ptr<Client> & client, unsigned int limit) {
    if (isOperator(client)) {
        //how many users can we handle in one channel?
        _user_limit = limit;
    }
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
    _clients.push_back(client);
    return (SUCCESS);
}

std::string Channel::getClientsNick() const {
    std::string clients;
    for (auto i = _clients.begin(); i != _clients.end(); i++) {
        clients.append((*i)->getName());
    }
    return (clients);
}

void    Channel::removeClient(const std::shared_ptr<Client> & client) {
    for (auto it = _clients.begin(); it != _clients.end(); it++) {
        if ((*it)->getClientSocket() == client->getClientSocket()) {
            _clients.erase(it);
            break;
        }
    }
}

int Channel::getSize() {
    return (_clients.size());
}

void    Channel::sendMsgToAll(const std::shared_ptr<Client>& client, std::string cmd, const std::optional<std::string>& target, const std::optional<std::string>& msg) {
    Message	message;

	for (auto it = this->_clients.begin(); it != this->_clients.end(); it++) {
		if (client != *it) {
			message.message(client, *it, cmd, target, msg);
        }
	}
}
std::string Channel::getTopic() const {
    return (_topic);
}
