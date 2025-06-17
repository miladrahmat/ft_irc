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
		std::string	_username;
		bool		_authenticated;
		std::string	_host_name;
		std::string	_buffer;

	public:
		Client(int socket);
		Client(Client&& old_client) noexcept;
		Client(Client const& old_client);
		~Client();
		int			getClientSocket() const;
		std::string	getRealName() const;
		std::string	getNickname() const;
		std::string	getUsername() const;
		std::string	getHostName() const;
		std::string	getBuffer() const;
		void		setClientSocket(int socket);
		void		setRealName(std::string& real_name);
		void		setUsername(std::string& user_name);
		void		setNickname(std::string& nickname);
		void		setHostName(std::string& host_name);
		void		appendBuffer(std::string& msg);
};