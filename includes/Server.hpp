#pragma once
#include <cstring>
#include <vector>
#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <bits/stdc++.h>
#include "Client.hpp"
#include "Message.hpp"
#include "reply.hpp"
#include "Channel.hpp"

class Server {
	private:
		std::string			_port;
		std::string			_password;
		int					_server_socket;
		std::vector<Client>	_client_vec;
		int					_epoll_fd;
		std::vector<Channel> _channels;

	public:
		Server(char** argv);
		~Server();
		void		start();
		std::string	getPort() const;
		std::string	getPassword() const;
		int			getServerSocket() const;
		int			getEpollFd() const;
		std::vector<Channel> getChannels() const;
		std::vector<Channel>::iterator	getChannel(std::string channel_name);
		void		handleNewClient();
		void		removeClient(Client& client);
		void    	receiveData(Client& client);
		void		changePut(Client& client, uint32_t put, int epoll_fd);
		void		parseInput(std::string msg, Client& client);
		bool		validateNick(std::string nickname);
		void		addNewChannel(std::string name, Client & client, std::string password = "");
};