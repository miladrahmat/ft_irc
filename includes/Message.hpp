#pragma once
#include <string>
#include "Client.hpp"
#include "Server.hpp"

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
	    void 		handleCap(Client& client);
        bool		getNextMessage(Client& client);
	    void		welcomeMessage(Client& client);
        void        errorMessage(Client& client, struct reply err);
};