#pragma once
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>

class Client {
	private:
		int			_client_socket;
		std::string	_real_name;
		std::string	_nickname;
		bool		_authenticated;
		std::string _username;
		std::string	_host_name;
		std::string	_buffer;
		std::string	_send_buffer;

	public:
		Client(int socket);
		Client(Client&& old_client) noexcept;
		Client(Client const& old_client);
		~Client();
		int			getClientSocket() const;
		bool		isAuthenticated() const;
		std::string	getRealName() const;
		std::string	getNickname() const;
		std::string	getUsername() const;
		std::string	getHostName() const;
		std::string	getBuffer() const;
		std::string	getSendBuffer() const;
		void		setClientSocket(int socket);
		void		setRealName(std::string& real_name);
		void		setUsername(std::string& user_name);
		void		setNickname(std::string& nickname);
		void		setHostName(std::string& host_name);
		void		authenticate();
		void		appendBuffer(std::string& msg);
		void		appendSendBuffer(std::string& msg);

		bool		receiveData();
		bool		sendData(); 
		bool		getNextMessage(std::string& msg);
};