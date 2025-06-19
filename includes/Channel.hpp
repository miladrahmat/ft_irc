
#pragma once

#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include "Client.hpp"

class Client;

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
        Channel(Channel const& other);
        ~Channel();
        Channel &operator=(const Channel& other) = default;
        bool isClient(const Client & client) const;
        bool isOperator(const Client & client) const;
        bool channelFull() const;
        std::string showTopic() const;
        void setTopic(const Client & client, std::string new_topic);
        void kickClient(const Client & client, const Client & client_to_kick);
        void inviteClient(const Client & client, const Client & new_client);
        void setInviteMode(const Client & client, bool set);
        void setTopicMode(const Client & client, bool set);
        void setChannelPassword(const Client & client, std::string password);
        void addOperator(const Client & client, const Client & new_operator);
        void removeOperator(const Client & client, const Client & operator_to_remove);
        void setUserLimit(const Client & client, unsigned int limit);
        void join(const Client & client, std::string password);
};