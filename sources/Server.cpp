
#include "Server.hpp"

bool server_stop = false;

Server::Server(char** argv) : _server_socket(-1), _state(nullptr) {
	struct addrinfo	hints, *res;
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	try {
		int	port = std::stoi(static_cast<std::string>(argv[1]));
		if (port < 1024 || port > 65535) {
			throw std::runtime_error("Port not within range (1024 - 65535)");
		}
		std::string	password = static_cast<std::string>(argv[2]);
		if (password.length() < 4) {
			throw std::runtime_error("Password should be at least 4 characters long");
		}
		_port = std::to_string(port);
		_password = password;
		int error = getaddrinfo(NULL, _port.c_str(), &hints, &res);
		if (error) {
			freeaddrinfo(res);
			throw std::runtime_error("getaddrinfo failed: " + std::string(gai_strerror(error)));
		}
		_server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (_server_socket < 0) {
			freeaddrinfo(res);
			throw std::runtime_error("Socker error");
		}
		int flags = fcntl(_server_socket, F_GETFL, 0);
    	if (flags < 0) {
			close(_server_socket);
			freeaddrinfo(res);
        	throw std::runtime_error("Error with fnctl (F_GETFL)");
    	}
    	if (fcntl(_server_socket, F_SETFL, flags | O_NONBLOCK) < 0) {
			close(_server_socket);
			freeaddrinfo(res);
        	throw std::runtime_error("Error with fcntl (F_SETFL)");
    	}
		int	opt = 1;
		setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		if (bind(_server_socket, res->ai_addr, res->ai_addrlen) < 0) {
			close(_server_socket);
			freeaddrinfo(res);
			throw std::runtime_error("Cannot bind socket");
		}
		freeaddrinfo(res);
		if (listen(_server_socket, SOMAXCONN) < 0) {
			close(_server_socket);
			throw std::runtime_error("listen failed");
		}
		std::cout << "Server started on port " << _port << std::endl;
		std::cout << "Server started with password " << _password << std::endl;
		signal(SIGINT, &stop);
		signal(SIGTSTP, &stop);
		_state = new State;
	} catch (std::exception& e) {
		if (_server_socket >= 0) {
			close(_server_socket);
		}
		throw std::runtime_error("Error: " + std::string(e.what()));
	}
}

Server::~Server() {
	if (_server_socket >= 0) {
		close(_server_socket);
	}
	if (_state != nullptr) {
		delete _state;
	}
	signal(SIGINT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
}

void Server::stop(int signum) {
	if (signum == SIGINT || signum == SIGTSTP)
		server_stop = true;
}

void Server::closeServer() {
	if (_server_socket >= 0) {
		close(_server_socket);
	}
	delete _state;
	_password.clear();
	_port.clear();
	signal(SIGINT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	exit(0);
}

std::string	Server::getPort() const {
	return (_port);
}

std::string	Server::getPassword() const {
	return (_password); }

int	Server::getServerSocket() const {
	return (_server_socket);
}

int	findIndex(std::vector<std::shared_ptr<Client>>& clients, int fd) {
	for (size_t i = 0; i < clients.size(); i++) {
		if (clients[i]->getClientSocket() == fd)
		{
			return (i);
		}
	}
	return (-1);
}

void Server::start() {
	int epoll_fd = epoll_create(1);
    if (epoll_fd < 0) {
		throw std::runtime_error("Error with epoll");
    }
    struct epoll_event ev_server;
    ev_server.events = EPOLLIN;
    ev_server.data.fd = _server_socket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev_server.data.fd, &ev_server) < 0) {
		close (epoll_fd);
		throw std::runtime_error("epoll_ctl failed: " + std::string(strerror(errno)));
    }
	try {
		struct epoll_event ev[64];
		while (true) {
			int eventsCount = epoll_wait(epoll_fd, ev, 64, 6000);
			if (server_stop) {
				close(epoll_fd);
				closeServer();
			}
			if (eventsCount < 0) {
				throw std::runtime_error("epoll_wait failed: " + std::string(strerror(errno)));
			}
			if (eventsCount > 0) {
				for (int i = 0; i < eventsCount; ++i) {
					int index = findIndex(_state->_clients, ev[i].data.fd);
					if (ev[i].data.fd == _server_socket) {
						handleNewClient(epoll_fd);
					}
					else if (ev[i].events & EPOLLIN) {
						receiveData(_state->_clients[index]);
					}
					else if (ev[i].events & EPOLLOUT) { 
						_state->_clients[index]->sendData();
					}
				}
			}
		}
    }
	catch (std::exception &e) {
		close(epoll_fd);
		throw std::runtime_error("Error: " + std::string(e.what()));
	}
    close(epoll_fd);
}

void Server::handleNewClient(int epoll_fd) {
    struct sockaddr_in client_addr;
    socklen_t   client_len = sizeof(client_addr);
	
	int client = accept(_server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client < 0) {
        std::cerr << "accept failed: "  + std::string(strerror(errno)) << std::endl;
		return ;
    }
	else {
        if (fcntl(client, F_SETFL, O_NONBLOCK) < 0) {
			close(client);
			throw std::runtime_error("fcntl failed: " + std::string(strerror(errno)));
        }
		std::string ip = inet_ntoa(client_addr.sin_addr);
        struct epoll_event ev2;
        ev2.events = EPOLLIN;
        ev2.data.fd = client;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &ev2) < 0) {
			close(client);
			throw std::runtime_error("epoll_ctl failed: " + std::string(strerror(errno)));
        }
		_state->_clients.push_back(std::make_shared<Client>(client, epoll_fd, ip));
    }
}

void Server::receiveData(std::shared_ptr<Client>& client) {
	Parser	parser;

	if (client.get() == nullptr) {
		return ;
	}
	if (!client->isValidPass()) {
		_state->removeClient(client);
		return ;
	}
	if (!client->receiveData()) {
		std::string input = ":Read error: Connection reset by peer";
		std::string command = "QUIT";
		std::unique_ptr<ACommand> cmd = parser.parseQuitCommand(client, command, input, *_state);
		if (cmd != nullptr) {
			cmd->execute();
		}
		return ;
	}
	Message	msg;
	if (!client->isAuthenticated()) {
		if (client->getBuffer().find("\r\n") != std::string::npos) {
			std::cout << "Incrementing registration attempts" << std::endl;
			client->incrementRegistrationAttempts();
		}
		if (client->getRegistrationAttempts() > 10) {
			std::cout << "REMOVING:" << std::endl;
			client->printClient();
			_state->removeClient(client);
			for (auto it = _state->_clients.begin(); it != _state->_clients.end(); ++it) {
				int i = 1;
				std::cout << "client number " << i << " is:" << std::endl;
				(*it)->printClient();
			}
			return ;
		}
	}
	while (msg.getNextMessage(client)) {
		msg.determineType(client);
		int	type = msg.getType();
		if (type == REG) {
			if (client->isAuthenticated()) {
				msg.codedMessage(client, *_state, ERR_ALREADYREGISTERED, {});
				continue ;
			}
			parser.parseRegisteration(client, msg.getMsg(), *_state);
			validatePassword(client);
			if (!client->isValidPass()) {
				return ;
			}
			validateClient(client);
		}
		else if (type == CMD) {
			if (!client->isAuthenticated()) {
				msg.codedMessage(client, *_state, ERR_NOTREGISTERED, {});
				std::cout << "Not a registered client, attepts: " << client->getRegistrationAttempts() << std::endl;
				continue ;
			}
			std::cout << client->getNickname() << ": " << msg.getMsg() << std::endl; 
			std::unique_ptr<ACommand> cmd = parser.parseCommand(client, msg.getMsg(), *_state);
			if (cmd != nullptr) {
				std::cout << "Executing " << cmd->getCommand() << std::endl;
				cmd->execute();
			}
		}
		else if (type == PING) {
			msg.messagePong(client, _state->_server_name, "PONG", _state->_server_name, _state->_server_name);
		}
		msg.clearMsg();
	}
}

bool Server::validatePassword(std::shared_ptr<Client>& client) {
	if (!client->isPassSet()) {
		return (true);
	}
	else if (client->getPassword() != _password && client->isValidPass()) {
		struct reply rpl = ERROR;
		rpl.msg = "Connection refused";
		Message msg;
		msg.codedMessage(client, *_state, ERR_PASSWDMISMATCH, {});
		msg.codedMessage(client, *_state, rpl, {});
		client->setValidPass(false);
		return (false);
	}
	return (true);
}

bool Server::validateClient(std::shared_ptr<Client>& client) {
	Message	msg;

	if (client->getPassword() != _password) {
		return (false);
	}
	client->authenticate();
	if (!client->isAuthenticated()) {
		return (false);
	}
	msg.welcomeMessage(client, *_state);
	client->printClient();
	return (true);
}