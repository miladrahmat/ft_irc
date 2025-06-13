#pragma once
#include <string>
#include <unistd.h>

class Client {
	private:
		int			_client_socket;
		std::string	_name;
		std::string	_nickname;

	public:
		Client(int socket);
		Client(Client&& old_client);
		~Client();
		int			getClientSocket() const;
		std::string	getName() const;
		std::string	getNickname() const;
		void		setClientSocket(int socket);
};