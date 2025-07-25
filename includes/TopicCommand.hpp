
#pragma once

#include "ACommand.hpp"
#include "reply.hpp"
#include "Message.hpp"

class TopicCommand : public ACommand {
	private:
		bool _set_topic;
		std::string	_channel;
		std::string	_topic;
		TopicCommand(std::string command, std::shared_ptr<Client>& client, State& state);

	public:
		static std::unique_ptr<ACommand> create(std::string command, std::shared_ptr<Client>& client,
			State& state, std::string channel, std::string topic);
		void execute() const override;
};
