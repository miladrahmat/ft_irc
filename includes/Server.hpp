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
#include "Client.hpp"

#define RPL_WELCOME 001
#define RPL_YOURHOST 002
#define RPL_CREATED 003
#define RPL_MYINFO 004

class Server {
	private:
		std::string			_port;
		std::string			_password;
		int					_server_socket;
		std::vector<Client>	_client_vec;

	public:
		Server(char** argv);
		~Server();
		void	start();
		std::string	getPort() const;
		std::string	getPassword() const;
		int			getServerSocket() const;
		void		handleNewClient(int epoll_fd);
		void    	receiveData(int fd, int epoll_fd);
		void		sendData(int fd, char *buf);
};