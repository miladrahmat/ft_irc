#pragma once
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>

class Client {
	private:
		int			_client_socket;
		int			_epoll_fd;
		std::string	_name;
		std::string	_nickname;
		std::string _username;
		std::string	_hostname;
		std::string	_password;
		bool		_authenticated;
		std::string	_buffer;
		std::string _send_buffer;
		std::vector<std::string> _invited_to;
		bool		_nick_validated;
		bool		_pass_validated;
		bool		_pass_set;
		int			_registration_attempts;
		int			_channel_count;

	public:
		Client(int socket, int epoll_fd, std::string ip);
		Client (const Client & old_client) = delete;
		Client(Client&& old_client) noexcept;
		~Client();
		Client		&operator=(const Client& other) = delete;
		bool		operator==(const Client& other) const;
		int			getClientSocket() const;
		int			getEpollFd() const;
		std::string getName() const;
		std::string	getNickname() const;
		std::string	getPassword() const;
		std::string	getHostname() const;
		std::string	getUsername() const;
		int			getRegistrationAttempts() const;
		bool		getNickValidated() const;
		bool		isAuthenticated() const;
		bool		isValidPass() const;
		bool		isInvited(const std::string& channel_name) const;
		bool		isPassSet() const;
		void 		setNickname(std::string nickname);
		void		setUsername(std::string username);
		void		setHostname(std::string hostname);
		void		setName(std::string name);
		void		setPassword(std::string password);
		void		setValidPass(bool set);
		void		setClientSocket(int socket);
		void		incrementRegistrationAttempts();
		void		validateNick();
		void		authenticate();
		void		setInvitedTo(std::string channel_name);
		void		appendBuffer(std::string const& msg);
		void		appendSendBuffer(std::string const& msg);
		void		changePut(uint32_t put);
		bool		receiveData();
		bool		sendData(); 
		std::string	getSendBuffer();
		std::string	getBuffer();
		void		emptyBuffer(int begin, int end);
		int			getChannelCount() const;
		void		incrementChannelCount();
		void		printClient() const;
};
