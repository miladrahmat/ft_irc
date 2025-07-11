
#pragma once

#include "ACommand.hpp"
#include "Message.hpp"

class ACommand;

class JoinCommand : public ACommand {
    private:
        std::vector<std::string> _channels; //at least one required, otherwise comma separated
        std::vector<std::string> _keys; //optional, comma separated
        JoinCommand(std::string command, std::shared_ptr<Client> & client, State & state);
    
    public:
        static std::unique_ptr<ACommand> create(std::string command, std::shared_ptr<Client>& client, State & state,
            std::vector<std::string> args);
        bool validChannelName(std::string channel) const;
        void execute() const override;
        void joinReply(std::string channel) const;
        void errReply(reply reply, std::string channel) const;
};