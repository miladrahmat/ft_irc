#pragma once
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>

class Client {
	private:
		int			_client_socket;
		std::string	_name;
		std::string	_nickname;
		std::string _username;
		std::string	_hostname;
		bool		_authenticated;
		std::string	_buffer;
		std::string _send_buffer;

	public:
		Client(int socket);
		Client (const Client & old_client);
		Client(Client&& old_client) noexcept;
		~Client();
		int			getClientSocket() const;
		std::string	getName() const;
		std::string	getNickname() const;
		bool		isAuthenticated() const;
		void 		setNickname(std::string nickname);
		void		setUsername(std::string username);
		void		setHostname(std::string hostname);
		void		setName(std::string name);
		void		setClientSocket(int socket);
		void		authenticate();
		void		appendBuffer(std::string& msg);
		void		appendSendBuffer(std::string& msg);
		bool		receiveData();
		bool		sendData(); 
		std::string	getSendBuffer();
		std::string	getBuffer();
		void		emptyBuffer(int begin, int end);
		void		printClient() const;
};