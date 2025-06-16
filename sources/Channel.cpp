
#include "Channel.hpp"

Channel::Channel(std::string name, Client client, std::string password) :
    _name(name), _clients{client}, _operators{client}, _password(password),
    _invite_only(false), _topic_command_access(true),
    _user_limit(std::numeric_limits<int>::max()) {};

Channel::~Channel() {};

bool Channel::isClient(const Client & client) const {
    if (std::find(_clients.begin(), _clients.end(), client) == _clients.end()) {
        return (false);
    }
    return (true);
};

bool Channel::isOperator(const Client & client) const {
    if (std::find(_operators.begin(), _operators.end(), client) == _operators.end()) {
        return (false);
    }
    return (true);
};

bool Channel::channelFull() const {
    if (_clients.size() == _user_limit) {
        return (true);
    }
    return (false);
}

std::string Channel::showTopic() const {
    //TODO just return the topic as string?
    return (_topic);
}

void Channel::setTopic(const Client & client, std::string new_topic) {
    if (isOperator(client) && _topic_command_access) {
        _topic = new_topic;
        return ;
    }
    //TODO how to handle if client is not operator or channel does not allow topic change
}

void Channel::kickClient(const Client & client, const Client & client_to_kick) {
    if (isOperator(client)) {
        if (isClient(client_to_kick)) {
            _clients.erase(std::find(_clients.begin(), _clients.end(), client));
            //TODO some message? what happens to the kicked out client?
        }
        //TODO what happens if the client to be kicked is not on the channel?
    }
    //TODO how to handle if client is not operator 
}

void Channel::inviteClient(const Client & client, const Client & new_client) {
    if (isOperator(client)) {
        if (channelFull() == false) {
            _clients.push_back(new_client);
        }
        //TODO channel full error message
    }
    //TODO how to handle if client is not operator
}

//set = true -> _invite_only = true, else set = false -> _invite_only = false
void Channel::setInviteMode(const Client & client, bool set) {
    if (isOperator(client)) {
        _invite_only = set;
    }
}

//set = true -> _topic_command_access = true, else set = false -> _topic_command_access = false
void Channel::setTopicMode(const Client & client, bool set) {
    if (isOperator(client)) {
        _topic_command_access = set;
    }
}

//if '/MODE -k' then set new_password to ""
void Channel::setChannelPassword(const Client & client, std::string new_password) {
    if (isOperator(client)) {
        //any rules for the password???
        _password = new_password;
    }
}

void Channel::addOperator(const Client & client, const Client & new_operator) {
    //do we get new_operator as Client or string? Do we here have to check if
    //it is a client of the channel here?
    if (isOperator(client)) {
        _operators.push_back(new_operator);
    }
}

void Channel::removeOperator(const Client & client, const Client & operator_to_remove) {
    //do we get operator_to_remove as Client or string?
    if (isOperator(client)) {
        if (isOperator(operator_to_remove)) {
            _operators.erase(std::find(_operators.begin(), _operators.end(), operator_to_remove));
        }
        //TODO operator_to_remove is not operator
    }
}

//limit 0 means no limit
void Channel::setUserLimit(const Client & client, unsigned int limit) {
    if (isOperator(client)) {
        //how many users can we handle in one channel?
        _user_limit = limit;
    }
}

void Channel::join(const Client & client, std::string password) {
    if (channelFull() == false) {
        if (isClient(client) == false) {
            if (password == _password) {
                _clients.push_back(client);
            }
        }
    }
}
