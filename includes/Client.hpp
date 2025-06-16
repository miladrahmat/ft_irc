#pragma once
#include <string>
#include <unistd.h>
#include <sys/socket.h>

class Client {
	private:
		int			_client_socket;
		std::string	_name;
		std::string	_nickname;
		std::string	_buffer;
		std::string _send_buffer;

	public:
		Client(int socket);
		Client(Client&& old_client);
		~Client();
		int			getClientSocket() const;
		std::string	getName() const;
		std::string	getNickname() const;
		void		setClientSocket(int socket);
		void		appendBuffer(std::string& msg);
		bool		receiveData(); //(from client)
		bool		sendData(); //(to server)
		bool		getNextMessage(std::string& msg);
		void		handleMessage(std::string msg);
};