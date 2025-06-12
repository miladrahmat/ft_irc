#pragma once
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>

class Server {
	private:
		std::string	_port;
		std::string	_password;
		int			_server_socket;

	public:
		Server(char** argv);
		~Server();
		void	start();
		std::string	getPort();
		std::string	getPassword();
		int			getServerSocket();
};