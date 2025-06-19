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

int	Server::getEpollFd() const {
	return (_epoll_fd);
}

int	Server::findIndex(int fd)
{
	for (size_t i = 0; i < _client_vec.size(); i++)
	{
		if (_client_vec[i].getClientSocket() == fd)
		{
			return (i);
		}
	}
	return (-1);
}

void	Server::start() {
	_epoll_fd = epoll_create(1);
    if (_epoll_fd < 0) {
        std::cerr << "Error with epoll" << std::endl;
        return ;
    }
    struct epoll_event ev_server;
    ev_server.events = EPOLLIN;
    ev_server.data.fd = _server_socket;
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, ev_server.data.fd, &ev_server) < 0) {
        std::cerr << "Error with epoll_ctl" << std::endl;
        return ;
    }
	struct epoll_event ev[64];
    while (true) {
        int eventsCount = epoll_wait(_epoll_fd, ev, 64, 6000); //timeout in milliseconds, not sure what it should be
        if (eventsCount < 0) {
            std::cerr << "Error with epoll_wait" << std::endl;
            break ;
        }
        if (eventsCount > 0) {
            for (int i = 0; i < eventsCount; ++i) {
				int index = findIndex(ev[i].data.fd);
                if (ev[i].data.fd == _server_socket) {
                    handleNewClient();
                }
				if (ev[i].events & EPOLLIN) {
					receiveData(_client_vec[index]);
				}
				if (ev[i].events & EPOLLOUT) { 
					_client_vec[index].sendData();
					if (_client_vec[index].getSendBuffer().empty()) {
						changePut(_client_vec[index], EPOLLIN, _epoll_fd);
					}
                }
			
            }
        }
    }
    close(_epoll_fd);
}

void Server::handleNewClient() {
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
        if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client, &ev2) < 0) {
            std::cerr << "Error with epoll_ctl (client)" << std::endl;
        }
		Client	new_client(client);
		_client_vec.push_back(std::move(new_client));
    }
}

void	Server::removeClient(Client& client) {
	struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = client.getClientSocket();
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, client.getClientSocket(), &ev)) {
		std::cerr << "Error with epoll_ctl: deleting client failed" << std::endl;
	}
	for (int i = 0; i < client.getChannelsSize(); i++) {
		//leave channel, client.getChannel(i)
	}
	for (size_t i = 0; i < _client_vec.size(); i++) {
		if (_client_vec[i].getClientSocket() == client.getClientSocket()) {
			//_client_vec.erase(_client_vec.begin() + i);
		}
	}
	close(client.getClientSocket());
}

void    Server::receiveData(Client& client)
{
	if (!client.receiveData()) {
		//client disconnected, handle it
		return ;
	}
	Message	msg;
	while (msg.getNextMessage(client)) {
		if (msg.handleCap(client))
		{
			msg.emptyMsg();
			changePut(client, EPOLLIN | EPOLLOUT, _epoll_fd);
		}
		else
			parseInput(msg.getMsg(), client);

	} 
}

bool	Server::validateNick(std::string nickname)
{
	std::string invalid_start = "$:#&~@+%";
	if (invalid_start.find(nickname[0]) != std::string::npos) {
		return (false);
	}
	std::string	invalid = " ,*?!@.";
	for (size_t i = 0; i < nickname.size(); i++) {
		for (int j = 0; j < 7; j++) {
			if (nickname[i] == invalid[j]) {
				return (false);
			}
		}
	}
	return (true);
}

void	Server::parseInput(std::string msg, Client& client)
{
	if (msg.substr(0, 4) == "PASS") {
		//check password
	}
	else if (msg.substr(0, 4) == "NICK"){
		//get nickname
		if (!validateNick(msg.substr(5, msg.size()))) { //this is not working
			//error invalid nickname, disconnect client
			std::cout  << "Invalid nickname" << std::endl;
			//should quit and disconnect here
		}
		client.setNickname(msg.substr(5, msg.size() - 1));
		//check if valid
		//invalid: ' ', ',', '*', '?', '!', '@',  '.'
		//invalid starting: '$', ':', '#', '&', '~', '+q' '+a' '@' '+o' '%' '+h' '+' '+v'

	}
	else if (msg.substr(0,4) == "USER") {
		int index = 0;
		for (size_t i = 5; i < msg.size(); i++)
		{
			if (msg[i] == ' ')
			{
				index = i;
				break ;
			}
		}
		client.setUsername(msg.substr(5, index - 5));
		for (size_t i = 5; i < msg.size(); i++)
		{
			if (msg[i] == ':')
			{
				index = i + 1;
				break ;
			}
		}
		client.setName(msg.substr(index, msg.size() - 1));
	}
}

void	Server::changePut(Client& client, uint32_t put, int epoll_fd) {
	struct epoll_event ev;
	ev.events = put;
	ev.data.fd = client.getClientSocket();
	epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client.getClientSocket(), &ev);
}