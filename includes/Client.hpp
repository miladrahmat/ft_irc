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
		std::string	_invited_to;

	public:
		Client(int socket);
		Client (const Client & old_client);
		Client(Client&& old_client) noexcept;
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
		void		setInvitedTo(std::string channel_name);
		std::string	getChannelInvitedTo() const;
};