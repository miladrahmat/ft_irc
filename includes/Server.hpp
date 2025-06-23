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
#include "Parser.hpp"

class Server {
	private:
		std::string			_port;
		std::string			_password;
		int					_server_socket;
		std::vector<Client>	_client_vec;
		int					_epoll_fd;

	public:
		Server(char** argv);
		~Server();
		void		start();
		std::string	getPort() const;
		std::string	getPassword() const;
		int			getServerSocket() const;
		int			getEpollFd() const;
		void		handleNewClient();
		void		removeClient(Client& client);
		void    	receiveData(Client& client);
		void		changePut(Client& client, uint32_t put, int epoll_fd);
		bool		validateClient(Client& client);
};