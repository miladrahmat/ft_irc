#pragma once
#include <string>
#include <memory>
#include "Client.hpp"
#include "Server.hpp"
#include "reply.hpp"

class Server;

enum    MSG_TYPE {
    CAP_START,
    CAP_REQ,
    CAP,
    CAP_END,
    CMD,
    MSG
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
        void        errorMessage(std::shared_ptr<Client>& client, struct reply err);
};