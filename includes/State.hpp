
#pragma once

#include <vector>

#include "Client.hpp"


class State {
    private:
        std::vector<Client> _clients;
        std::vector<Channel> _channels;
    public:
        std::vector<Channel>::iterator getChannel(std::string channel_name);
        std::vector<Channel> & getChannels();
        void addNewChannel(std::string name, Client & client, std::string password = "");

        friend class Server;
};