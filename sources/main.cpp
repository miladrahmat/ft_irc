#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <unistd.h>
#include "Server.hpp"


int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Expected: ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    Server  ircserv(argv);
    if (ircserv.getError()) {
        std::cout << "Error in constructor" << std::endl;
        return (1);
    }
    ircserv.start();
    if (ircserv.getError()) {
        return (1);
    }
    /*struct sockaddr_in addr; //this or addrinfo?? (addrinfo version in constructor)
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(stoi(ircserv.getPort())); //port here*/

            // struct sockaddr {
            //        sa_family_t     sa_family;      /* Address family */
            //        char            sa_data[];      /* Socket address */
            //    };
    //socklen_t addrlen = 125;    //no idea what this should be
    return (0);
}