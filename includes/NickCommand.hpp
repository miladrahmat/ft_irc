
#pragma once

#include "ACommand.hpp"
#include "reply.hpp"
#include "Message.hpp"

class NickCommand : public ACommand {
	private:
		std::string	_nickname;
		reply		_reply;
		bool		_error;
		NickCommand(std::string command, std::shared_ptr<Client>& client, State& state);

	public:
		static std::unique_ptr<ACommand> create(std::string command, std::shared_ptr<Client>& client, State& state,
			std::string nick);
		bool execute() const override;
};