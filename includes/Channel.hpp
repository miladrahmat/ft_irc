
#pragma once

#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <memory>
#include "Client.hpp"

class Client;

class Channel {
    private:
        const std::string _name;
        std::vector<std::shared_ptr<Client>> _clients;
        std::vector<std::shared_ptr<Client>> _operators;
        std::string _topic;
        std::string _password;
        bool _invite_only;
        bool _topic_command_access;
        int _user_limit;
    public:
        Channel(std::string name, std::shared_ptr<Client> client, std::string password = "");
        ~Channel();
        bool isClient(const std::shared_ptr<Client> & client) const;
        bool isOperator(const std::shared_ptr<Client> & client) const;
        bool channelFull() const;
        std::string showTopic() const;
        void setTopic(const std::shared_ptr<Client> & client, std::string new_topic);
        void kickClient(const std::shared_ptr<Client> & client, const std::shared_ptr<Client> & client_to_kick);
        bool inviteClient(const std::shared_ptr<Client> & client, std::shared_ptr<Client> & new_client);
        void setInviteMode(const std::shared_ptr<Client> & client, bool set);
        void setTopicMode(const std::shared_ptr<Client> & client, bool set);
        void setChannelPassword(const std::shared_ptr<Client> & client, std::string password);
        void addOperator(const std::shared_ptr<Client> & client, const std::shared_ptr<Client> & new_operator);
        void removeOperator(const std::shared_ptr<Client> & client, const std::shared_ptr<Client> & operator_to_remove);
        void setUserLimit(const std::shared_ptr<Client> & client, unsigned int limit);
        bool join(const std::shared_ptr<Client> & client, std::string password);
        std::string getName() const;
        std::string getTopic() const;
        std::string getClientsNick() const;
};
