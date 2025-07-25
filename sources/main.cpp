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
    try {    
        Server  ircserv(argv);
        ircserv.start();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0);
}