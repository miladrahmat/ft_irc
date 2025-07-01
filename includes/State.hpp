
#pragma once

#include <vector>
#include <memory>
#include "Client.hpp"


class State {
    private:
        int _epoll_fd;
        std::vector<std::shared_ptr<Client>> _clients;
        std::vector<Channel> _channels;
    public:
    	int     getEpollFd() const;
        std::vector<Channel>::iterator getChannel(std::string channel_name);
        std::vector<Channel> & getChannels();
        void addNewChannel(std::string name, std::shared_ptr<Client> & client, std::string password = "");
        std::vector<std::shared_ptr<Client>>::iterator  getClient(std::string nickname);
        std::vector<std::shared_ptr<Client>> &  getClients();

        friend class Server;
};