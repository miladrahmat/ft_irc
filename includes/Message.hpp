
#pragma once
#include <string>
#include <memory>
#include <optional>
#include "Client.hpp"
#include "State.hpp"
#include "reply.hpp"

class State;

enum MSG_TYPE {
    CAP_LS,
    CAP_REQ,
    CAP_REQ_AGAIN,
    CAP_END,
    CMD,
    PING
};

class Message {
    private:
        std::string _msg;
        std::string _send_msg;
        int _type;

    public:
        Message();
        ~Message();
        std::string & getMsg();
        int getType() const;
        void clearMsg();
        void clearSendMsg();
	    void determineType(std::shared_ptr<Client>& client);
        bool getNextMessage(std::shared_ptr<Client>& client);
	    void welcomeMessage(std::shared_ptr<Client>& client, State& state);
        void messagePong(std::shared_ptr<Client>& client, std::string sender,
            std::string command, std::string target, std::string msg);
        void messageCap(std::shared_ptr<Client>& client);
        void message(const std::shared_ptr<Client>& s_client, std::shared_ptr<Client> & r_client,
            std::string cmd, const std::optional<std::string>& target,
            const std::optional<std::string>& msg);
        void codedMessage(std::shared_ptr<Client>& client, State& state, struct reply code,
            const std::optional<std::string>& target);
};