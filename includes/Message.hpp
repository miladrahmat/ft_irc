#pragma once
#include <string>
#include "Client.hpp"
#include "Server.hpp"
#include "reply.hpp"

class Server;

class Message {
    private:
        std::string _msg;
        std::string _send_msg;
    public:
        Message();
        ~Message();
        std::string getMsg();
        void        emptyMsg();
	    bool		handleCap(Client& client);
        bool		getNextMessage(Client& client);
	    void		welcomeMessage(Client& client, int i);
        void        errorMessage(Client& client, reply err);
};