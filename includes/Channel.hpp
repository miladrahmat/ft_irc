
#pragma once

#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <memory>
#include <optional>
#include <ctime>
#include "Message.hpp"
#include "Client.hpp"
#include "reply.hpp"

class Client;

class Channel {
    private:
        std::string _name;
        std::string _password;  //"" means no password
        bool _invite_only;
        bool _topic_command_access;     //true: only operators can change, false: everybody can change
        int _user_limit;    //-1 means no limit

    public:
        std::vector<std::shared_ptr<Client>> clients;
        std::vector<std::shared_ptr<Client>> operators;
        std::string topic;
        std::shared_ptr<Client> topic_who;
        std::time_t topic_when;

        ~Channel();
        Channel(std::string name, std::shared_ptr<Client> client, std::string password = "");
        
        std::string getName() const;
        bool getTopicMode() const;
        int getSize();
        int getUserLimit();
        std::string getPassword() const;
        std::string getModes(const std::shared_ptr<Client> & client);
        std::vector<std::shared_ptr<Client>>::iterator getClient(std::string nickname);

        void setTopic(const std::shared_ptr<Client> & client, std::string new_topic);
        reply setInviteMode(std::shared_ptr<Client> & client, bool set_to);
        reply setTopicMode(const std::shared_ptr<Client> & client, bool set_to);
        reply checkPrivileges(const std::shared_ptr<Client> & client) const;
        bool isClient(const std::shared_ptr<Client> & client) const;
        bool isOperator(const std::shared_ptr<Client> & client) const;
        bool channelFull(const std::shared_ptr<Client> & client) const;
        reply kickClient(const std::shared_ptr<Client> & client, const std::shared_ptr<Client> & client_to_kick, std::string msg);
        reply inviteClient(const std::shared_ptr<Client> & client, std::shared_ptr<Client> & new_client);
        reply setChannelPassword(const std::shared_ptr<Client> & client, std::string password);
        reply addOperator(const std::shared_ptr<Client> & client, const std::shared_ptr<Client> & new_operator);
        reply removeOperator(const std::shared_ptr<Client> & client, const std::shared_ptr<Client> & operator_to_remove);
        reply setUserLimit(const std::shared_ptr<Client> & client, unsigned int limit);
        reply join(const std::shared_ptr<Client> & client, std::string password);
        void removeClient(const std::shared_ptr<Client> & client);
        void sendMsgToAll(const std::shared_ptr<Client>& client, std::string cmd, const std::optional<std::string>& target, const std::optional<std::string>& msg);
};
