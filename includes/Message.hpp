#pragma once
#include <string>
#include <memory>
#include <optional>
#include "Client.hpp"
#include "reply.hpp"

class Server;

enum    MSG_TYPE {
    CAP_START,
    CAP_REQ,
    CAP,
    CAP_END,
    CMD
};

class Message {
    private:
        std::string _msg;
        std::string _send_msg;
        int         _type;

    public:
        Message();
        ~Message();
        std::string& getMsg();
        int          getType() const;
        void        emptyMsg();
	    void 		handleCap(std::shared_ptr<Client>& client);
        bool		getNextMessage(std::shared_ptr<Client>& client);
	    void		welcomeMessage(std::shared_ptr<Client>& client);
        void        message(std::shared_ptr<Client>& s_client, std::shared_ptr<Client> & r_client, std::string cmd, const std::optional<std::string>& target, const std::optional<std::string>& msg);
        void        codedMessage(std::shared_ptr<Client>& client, struct reply code, const std::optional<std::string>& target);
};