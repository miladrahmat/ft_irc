
#pragma once

#include <optional>
#include "ACommand.hpp"
#include "Message.hpp"

/*
· i: Set/remove Invite-only channel (no param)
· t: Set/remove the restrictions of the TOPIC command to channel (no param)
operators
· k: Set/remove the channel key (password) (the key as param)
· o: Give/take channel operator privilege (nick as param)
· l: Set/remove the user limit to channel (limit as param)
*/

class ModeCommand : public ACommand {

    private:
        enum mode_action {
            ADD = '+',
            REMOVE = '-'
        };
        enum mode_type {
            INVITE = 'i',
            TOPIC = 't',
            KEY = 'k',
            OPERATOR = 'o',
            LIMIT = 'l',
            UNKNOWN
        };
        mode_type _mode;
        mode_action _action;
        std::vector<Channel>::iterator _channel_it;    //channel that the mode will affect
        std::string _key;   //param for +k
        std::string _nick;  //param for +/-o
        int _limit;    //param for +l
        ModeCommand(std::string command, std::shared_ptr<Client> & client, State & state);
        void setModeAction(char mode_action);
        void setMode(char mode);
        int checkTarget(std::string & target);

    public:
        static std::unique_ptr<ACommand> create(std::string command, std::shared_ptr<Client>& client,
            State & state, std::string target, std::string mode, std::optional<std::string> mode_param);
        bool execute() const override;
};