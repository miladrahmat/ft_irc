#pragma once
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <vector>
#include "Channel.hpp"

class Channel;

class Client {
	private:
		int			_client_socket;
		std::string	_name;
		std::string	_nickname;
		std::string _username;
		std::string	_buffer;
		std::string _send_buffer;
		std::vector<Channel*> _channels;

	public:
		Client(int socket);
		Client (const Client & old_client);
		Client(Client&& old_client) noexcept;
		~Client();
		Client		&operator=(const Client& other);
		bool		operator==(const Client& other) const;
		int			getClientSocket() const;
		std::string	getName() const;
		std::string	getNickname() const;
		Channel		getChannel(int i) const;
		int			getChannelsSize() const;
		void 		setNickname(std::string nickname);
		void		setUsername(std::string username);
		void		setName(std::string name);
		void		setClientSocket(int socket);
		void		appendBuffer(std::string& msg);
		void		appendSendBuffer(std::string& msg);
		bool		receiveData();
		bool		sendData(); 
		std::string	getSendBuffer();
		std::string	getBuffer();
		void		emptyBuffer(int begin, int end);
};