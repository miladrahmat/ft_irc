#pragma once
#include <string>
#include <unistd.h>

class Client {
	private:
		int			_client_socket;
		std::string	_real_name;
		std::string	_nickname;
		std::string	_user_name;
		std::string	_host_name;
		std::string	_buffer;

	public:
		Client(int socket);
		Client(Client&& old_client);
		~Client();
		int			getClientSocket() const;
		std::string	getRealName() const;
		std::string	getNickname() const;
		std::string	getUserName() const;
		std::string	getHostName() const;
		std::string	getBuffer() const;
		void		setClientSocket(int socket);
		void		setRealName(std::string& real_name);
		void		setUserName(std::string& user_name);
		void		setNickName(std::string& nickname);
		void		setHostName(std::string& host_name);
		void		appendBuffer(std::string& msg);

};