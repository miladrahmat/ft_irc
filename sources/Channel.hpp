
#pragma once

#include <string>
#include <vector>
#include <limits>
#include "Client.hpp"

class Channel {
    private:
        std::string _name;
        std::vector<Client> _clients;
        std::vector<Client> _operators;
        std::string _topic;
        std::string _password;
        bool _invite_only;
        bool _topic_command_access;
        int _user_limit;
    public:
        Channel(std::string name, Client client, std::string password = "");
        ~Channel();
        bool isClient(const Client & client) const;
        bool isOperator(const Client & client) const;
        bool isChannelFull() const;
};