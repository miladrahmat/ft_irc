#include "Server.hpp"

Server::Server(char** argv) {
	struct addrinfo	hints, *res;
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	try {
		int	port = std::stoi(static_cast<std::string>(argv[1]));
		if (port < 1024 || port > 65535) {
			std::cerr << "Error: Port not within range (1024 - 65535)" << std::endl;
			exit(1);
		}
		std::string	password = static_cast<std::string>(argv[2]);
		if (password.length() < 4) {
			std::cerr << "Error: Password should be at least 4 characters long" << std::endl;
			exit(1);
		}
		_port = std::to_string(port);
		_password = password;
		int error = getaddrinfo(NULL, _port.c_str(), &hints, &res);
		if (error) {
			std::cerr << "Error: getaddrinfo failed: " << gai_strerror(error) << std::endl;
			exit(1);
		}
		_server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (_server_socket < 0) {
			std::cerr << "Socker error" << std::endl;
			freeaddrinfo(res);
			exit(1);
		}
		int flags = fcntl(_server_socket, F_GETFL, 0);
    	if (flags < 0) {
        	std::cerr << "Error with fnctl (F_GETFL)" << std::endl;
        	exit (1);
    	}
    	if (fcntl(_server_socket, F_SETFL, flags | O_NONBLOCK) < 0) {
        	std::cerr << "Error with fcntl (F_SETFL)" << std::endl;
        	exit (1);
    	}
		int	opt = 1;
		setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		if (bind(_server_socket, res->ai_addr, res->ai_addrlen) < 0) {
			std::cerr << "Error: Cannot bind socket" << std::endl;
			freeaddrinfo(res);
			close(_server_socket);
			exit(1);
		}
		freeaddrinfo(res);
		if (listen(_server_socket, SOMAXCONN) < 0) {
			std::cerr << "Error: listen failed" << std::endl;
			exit(1);
		}
		std::cout << "Server started on port " << _port << std::endl;
		std::cout << "Server started with password " << _password << std::endl;
	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

Server::~Server() {
	close(_server_socket);
}

std::string	Server::getPort() const {
	return (_port);
}

std::string	Server::getPassword() const {
	return (_password); }

int	Server::getServerSocket() const {
	return (_server_socket);
}

void	Server::start() {
	int epoll_fd = epoll_create(1);
    if (epoll_fd < 0) {
        std::cerr << "Error with epoll" << std::endl;
        return ;
    }
    struct epoll_event ev_server;
    ev_server.events = EPOLLIN;
    ev_server.data.fd = _server_socket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev_server.data.fd, &ev_server) < 0) {
        std::cerr << "Error with epoll_ctl" << std::endl;
        return ;
    }
	struct epoll_event ev[64];
    while (true) {
        int eventsCount = epoll_wait(epoll_fd, ev, 64, 6000); //timeout in milliseconds, not sure what it should be
        if (eventsCount < 0) {
            std::cerr << "Error with epoll_wait" << std::endl;
            break ;
        }
        if (eventsCount > 0) {
            for (int i = 0; i < eventsCount; ++i) {
				int fd = ev[i].data.fd;
                if (ev[i].data.fd == _server_socket) {
                    handleNewClient(epoll_fd);
                } else {
					receiveData(_client_vec[fd]);
				}
				//if (ev[i].events & EPOLLIN) {
					//receiveData(_client_vec[fd]);
					//}
				//if (ev[i].events & EPOLLOUT) { 
				//	_client_vec[fd].sendData();
                //}
			
            }
        }
    }
    close(epoll_fd);
}

void Server::handleNewClient(int epoll_fd) {
    struct sockaddr_in client_addr;
    socklen_t   client_len = sizeof(client_addr);
	int client = accept(_server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client < 0) {
        std::cerr << "Error with accept" << std::endl;
    } else {
        if (fcntl(client, F_SETFL, O_NONBLOCK) < 0) {
            std::cerr << "Error with fcntl (client)" << std::endl;
            return ;
        }
        struct epoll_event ev2;
        ev2.events = EPOLLIN;
        ev2.data.fd = client;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &ev2) < 0) {
            std::cerr << "Error with epoll_ctl (client)" << std::endl;
        }
		Client	new_client(client);
		_client_vec.push_back(std::move(new_client));
		std::cout << "new client connected!" << std::endl;
		receiveData(new_client);
		//std::string welcome = "Welcome to IRC!\r\n";
		//send(client, welcome.c_str(), welcome.size(), 0);
    }
}

void    Server::receiveData(Client& client)
{
	if (!client.receiveData()) {
		std::cout << "HELLO RECEIVING HERE" << std::endl;
		//client disconnected, handle it
		return ;
	}
	std::string msg;
	while (client.getNextMessage(msg)) {
		client.handleMessage(msg);
		//message ready, handle command
	} 
}

//message format: :ircserv NUMERIC_RESPONSE client_nick :message \r\n
//":ircserv " + NUMERIC_RESPONSE + client._nickname + ":" + msg;

//void    Server::sendData(int fd, char *buf)
//{
    //std::string message(buf);
	//fd = clients socket
	
    //int sent_bytes = send(fd, message.c_str(), message.length(), MSG_DONTWAIT);
    //if (sent_bytes < 0)
    //{
    //    std::cerr << "Error: failed to sen data to user" << std::endl;
    //}
    //else
    //{
    //    std::cout << "Data succesfully sent to client" << std::endl;
    //}
//}