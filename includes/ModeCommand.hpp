
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
        struct mode_struct {
            mode_action action;
            mode_type mode;
            std::string param;
        };
        std::vector<mode_struct> modes;
        std::vector<Channel>::iterator _channel_it;
        ModeCommand(std::string command, std::shared_ptr<Client> & client, State & state);
        void setModeAction(mode_struct & mode, char mode_action);
        void setMode(mode_struct & mode_struct, char mode);
        int checkTarget(std::string & target);
        void executeEmptyMode(Channel & channel) const;
        void executeKey(Channel & channel, const mode_struct & mode_obj) const;
        void executeInvite(Channel & channel, const mode_struct & mode_obj) const;
        void executeTopic(Channel & channel, const mode_struct & mode_obj) const;
        void executeLimit(Channel & channel, const mode_struct & mode_obj) const;
        void executeOperator(Channel & channel, const mode_struct & mode_obj) const;

    public:
        static std::unique_ptr<ACommand> create(std::string command, std::shared_ptr<Client>& client,
            State & state, std::string target, std::string modes, std::string params);
        bool execute() const override;
};