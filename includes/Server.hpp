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
#include "Parser.hpp"
#include "State.hpp"

class Server {
	private:
		std::string			_port;
		std::string			_password;
		int					_server_socket;
		//int					_epoll_fd;
		State				_state;

	public:
		Server(char** argv);
		~Server();
		void		start();
		std::string	getPort() const;
		std::string	getPassword() const;
		int			getServerSocket() const;
		std::vector<Channel> getChannels() const;
		void		handleNewClient(int epoll_fd);
		void		removeClient(std::shared_ptr<Client>& client);
		void    	receiveData(std::shared_ptr<Client>& client);
		void		parseInput(std::string msg, std::shared_ptr<Client>& client);
		bool		validateNick(std::string nickname);
		bool		validateClient(std::shared_ptr<Client>& client);
};
