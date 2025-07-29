#pragma once

#include "ACommand.hpp"
#include "reply.hpp"
#include "Message.hpp"

class WhoCommand : public ACommand {
    private:
        std::string _channel;
        std::string _nick;
        WhoCommand(std::string command, std::shared_ptr<Client>& client, State& state);
        void sendReply(const std::shared_ptr<Client>& client, const std::string& channel, const std::string& flags) const;
    public:
        static std::unique_ptr<ACommand> create(std::string command, std::shared_ptr<Client>& client,
			State& state, std::string arg);
		void execute() const override;
};