#pragma once
#include <string>
#include <memory>
#include <optional>
#include "Client.hpp"
#include "reply.hpp"

class Server;

enum    MSG_TYPE {
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
        int         _type;

    public:
        Message();
        ~Message();
        std::string& getMsg();
        int          getType() const;
        void        clearMsg();
        void	    clearSendMsg();
	    void 		determineType(std::shared_ptr<Client>& client);
        bool		getNextMessage(std::shared_ptr<Client>& client);
	    void		welcomeMessage(std::shared_ptr<Client>& client);
        void        message(std::shared_ptr<Client>& s_client, std::shared_ptr<Client> & r_client, std::string cmd, const std::optional<std::string>& target, const std::optional<std::string>& msg);
        void        messageCap(std::shared_ptr<Client>& client);
        void        codedMessage(std::shared_ptr<Client>& client, struct reply code, const std::optional<std::string>& target);
};