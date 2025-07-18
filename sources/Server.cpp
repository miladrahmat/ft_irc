
#include "Server.hpp"

bool server_stop = false;

Server::Server(char** argv) {
	struct addrinfo	hints, *res;
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	_server_socket = -1;
	_state = new State;
	_error = false;

	try {
		int	port = std::stoi(static_cast<std::string>(argv[1]));
		if (port < 1024 || port > 65535) {
			std::cerr << "Error: Port not within range (1024 - 65535)" << std::endl;
			closeServer(1);
		}
		std::string	password = static_cast<std::string>(argv[2]);
		if (password.length() < 4) {
			std::cerr << "Error: Password should be at least 4 characters long" << std::endl;
			closeServer(1);
		}
		_port = std::to_string(port);
		_password = password;
		int error = getaddrinfo(NULL, _port.c_str(), &hints, &res);
		if (error) {
			std::cerr << "Error: getaddrinfo failed: " << gai_strerror(error) << std::endl;
			freeaddrinfo(res);
			closeServer(1);
		}
		_server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (_server_socket < 0) {
			std::cerr << "Socker error" << std::endl;
			freeaddrinfo(res);
			closeServer(1);
		}
		int flags = fcntl(_server_socket, F_GETFL, 0);
    	if (flags < 0) {
        	std::cerr << "Error with fnctl (F_GETFL)" << std::endl;
			freeaddrinfo(res);
        	closeServer(1);
    	}
    	if (fcntl(_server_socket, F_SETFL, flags | O_NONBLOCK) < 0) {
        	std::cerr << "Error with fcntl (F_SETFL)" << std::endl;
			freeaddrinfo(res);
        	closeServer(1);
    	}
		int	opt = 1;
		setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		if (bind(_server_socket, res->ai_addr, res->ai_addrlen) < 0) {
			std::cerr << "Error: Cannot bind socket" << std::endl;
			freeaddrinfo(res);
			closeServer(1);
		}
		freeaddrinfo(res);
		if (listen(_server_socket, SOMAXCONN) < 0) {
			std::cerr << "Error: listen failed" << std::endl;
			closeServer(1);
		}
		std::cout << "Server started on port " << _port << std::endl;
		std::cout << "Server started with password " << _password << std::endl;
		signal(SIGINT, &stop);
		signal(SIGTSTP, &stop);
	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		closeServer(1);
		return ;
	}
}

bool Server::getError() const {
	return (_error);
}

Server::~Server() {}

void Server::stop(int signum) {
	if (signum == SIGINT || signum == SIGTSTP)
		server_stop = true;
}

void Server::closeServer(int ret) {
	if (_server_socket >= 0)
		close(_server_socket);
	for (auto it = _state->_channels.begin(); it != _state->_channels.end(); ++it) {
		it->clients.clear();
		it->operators.clear();
	}
	_state->_channels.clear();
	for (auto it = _state->_clients.begin(); it != _state->_clients.end(); ++it) {
		it->reset();
	}
	_state->_clients.clear();
	delete _state;
	_password.clear();
	_port.clear();
	signal(SIGINT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	if (ret > 0) {
		_error = true;
		return ;
	}
	exit(ret);
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
        std::cerr << "Error with epoll" << std::endl;
		closeServer(1);
        return ;
    }
    struct epoll_event ev_server;
    ev_server.events = EPOLLIN;
    ev_server.data.fd = _server_socket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev_server.data.fd, &ev_server) < 0) {
        std::cerr << "Error with epoll_ctl" << std::endl;
		closeServer(1);
        return ;
    }
	try {
		struct epoll_event ev[64];
		while (true) {
			int eventsCount = epoll_wait(epoll_fd, ev, 64, 6000);
			if (server_stop) {
				close(epoll_fd);
				closeServer(0);
			}
			if (eventsCount < 0) {
				std::cerr << "Error with epoll_wait" << std::endl;
				break ;
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
		std::cerr << "Error: " << e.what() << std::endl;
		close(epoll_fd);
		closeServer(1);
		return ;
	}
    close(epoll_fd);
}

void Server::handleNewClient(int epoll_fd) {
    struct sockaddr_in client_addr;
    socklen_t   client_len = sizeof(client_addr);
	
	int client = accept(_server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client < 0) {
        std::cerr << "Error with accept" << std::endl;
    }
	else {
        if (fcntl(client, F_SETFL, O_NONBLOCK) < 0) {
            std::cerr << "Error with fcntl (client)" << std::endl;
			close(client);
            return ;
        }
		std::string ip = inet_ntoa(client_addr.sin_addr);
        struct epoll_event ev2;
        ev2.events = EPOLLIN;
        ev2.data.fd = client;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &ev2) < 0) {
            std::cerr << "Error with epoll_ctl (client)" << std::endl;
			close(client);
			return ;
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
		std::string input = "QUIT :Read error: Connection reset by peer";
		std::unique_ptr<ACommand> cmd = parser.parseQuitCommand(client, input, *_state);
		if (cmd != nullptr) {
			cmd->execute();
		}
		return ;
	}
	Message	msg;
	while (msg.getNextMessage(client)) {
		msg.determineType(client);
		int	type = msg.getType();
		if (type == CAP_LS || type == CAP_REQ || type == CAP_REQ_AGAIN || type == CAP_END) {
			clientRegisteration(client, msg);
			validatePassword(client);
			if (!client->isValidPass()) {
				return ;
			}
		}
		else if (type == CMD) {
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

void Server::clientRegisteration(std::shared_ptr<Client>& client, Message& msg) {
	Parser parser;

	if (!client->isValidPass()) {
		return ;
	}
	int	type = msg.getType();
	if (type == CAP_LS) {
		parser.parseCap(client, msg.getMsg(), *_state);
		msg.messageCap(client);
	}
	else if (type == CAP_REQ || type == CAP_REQ_AGAIN) {
		if (type == CAP_REQ_AGAIN) {
			std::unique_ptr<ACommand> cmd = parser.parseNickCommand(client, msg.getMsg(), *_state);
			if (cmd != nullptr) {
				std::cout << "Executing " << cmd->getCommand() << std::endl;
				cmd->execute();
			}
			if (!client->getNickValidated()) {
				msg.clearSendMsg();
				return ;
			}
		}
		if (client->getNickname().empty()) {
			msg.clearSendMsg();
		}
		msg.messageCap(client);
	}
	else if (type == CAP_END) {
		if (!validateClient(client)) {
			msg.clearSendMsg();
			return ;
		}
	}
}

bool Server::validatePassword(std::shared_ptr<Client>& client) {
	if (client->getPassword().empty()) {
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
	msg.welcomeMessage(client, *_state);
	client->authenticate();
	if (!client->isAuthenticated()) {
		return (false);
	}
	client->printClient();
	return (true);
}