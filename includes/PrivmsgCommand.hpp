
#pragma once

#include "ACommand.hpp"
#include "Message.hpp"

class PrivmsgCommand : public ACommand {
	private:
		std::string _msg;
		std::string _msg_to;
		bool _channel;
		PrivmsgCommand(std::string command, std::shared_ptr<Client>& client, State& state);

	public:
		static std::unique_ptr<ACommand> create(std::string command, std::shared_ptr<Client>& client,
			State& state, std::string target, std::string msg);
		void execute() const override;
};