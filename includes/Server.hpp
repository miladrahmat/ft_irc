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
#include "Channel.hpp"

#define RPL_WELCOME "001" //Welcome to IRC
#define RPL_YOURHOST "002" //Your host is ircserv
#define RPL_CREATED "003" //This server was created today
#define RPL_MYINFO "004" //ircserv 1.0

class Server {
	private:
		std::string				_port;
		std::string				_password;
		int						_server_socket;
		std::vector<Client>		_client_vec;
		std::vector<Channel>	_channels;
		bool isExistingChannel(const std::stirng channel_name);

	public:
		Server(char** argv);
		~Server();
		void	start();
		std::string	getPort() const;
		std::string	getPassword() const;
		int			getServerSocket() const;
		void		handleNewClient(int epoll_fd);
		void    	receiveData(Client& client); 
		//void		sendData(int fd, char *buf);
		void		joinCommand(const Client& client, std::vector<std::string> channel_names,
						std::vector<std::string> passwords);
};