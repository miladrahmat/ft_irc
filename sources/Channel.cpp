
#include "Channel.hpp"

Channel::Channel(std::string name, Client client, std::string password) :
    _name(name), _clients{client}, _operators{client}, _invite_only(false),
    _topic_command_access(true), _user_limit(std::numeric_limits<int>::max()) {

}

Channel::~Channel() {

}
bool isClient(const Client & client) const;
bool isOperator(const Client & client) const;
bool isChannelFull() const;