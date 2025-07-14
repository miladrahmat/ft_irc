
#pragma once

#include <vector>
#include <memory>
#include "Client.hpp"
#include "Channel.hpp"
#include "reply.hpp"

class Channel;

class State {
    private:
        const std::string _server_name = "ircserv.galleria";
        std::vector<std::shared_ptr<Client>> _clients;
        std::vector<Channel> _channels;
    public:
        std::string getServerName() const;
        std::vector<Channel>::iterator getChannel(std::string channel_name);
        std::vector<Channel> & getChannels();
        void removeClient(std::shared_ptr<Client>& client);
        reply addNewChannel(std::string name, std::shared_ptr<Client> & client, std::string password = "");
        std::vector<std::shared_ptr<Client>>::iterator  getClient(std::string nickname);
        std::vector<std::shared_ptr<Client>> &  getClients();

        friend class Server;
};