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
		std::string	_hostname;
		std::string	_password;
		bool		_authenticated;
		std::string	_buffer;
		std::string _send_buffer;
		std::string _invited_to;
		std::vector<Channel*> _channels;

	public:
		Client(int socket);
		Client (const Client & old_client) = delete;
		Client(Client&& old_client) noexcept;
		~Client();
		Client		&operator=(const Client& other) = delete;
		bool		operator==(const Client& other) const;
		int			getClientSocket() const;
		std::string	getName() const;
		std::string	getNickname() const;
		std::string	getPassword() const;
		std::string	getHostname() const;
		std::string	getUsername() const;
		bool		isAuthenticated() const;
		std::string getChannelInvitedTo() const;
		void 		setNickname(std::string nickname);
		bool		validateNickname(std::string nickname);
		void		setUsername(std::string username);
		void		setHostname(std::string hostname);
		void		setName(std::string name);
		void		setPassword(std::string password);
		void		setClientSocket(int socket);
		void		authenticate();
		void		setInvitedTo(std::string channel_name);
		void		appendBuffer(std::string const& msg);
		void		appendSendBuffer(std::string const& msg);
		bool		receiveData();
		bool		sendData(); 
		std::string	getSendBuffer();
		std::string	getBuffer();
		void		emptyBuffer(int begin, int end);
		void		printClient() const;
};
