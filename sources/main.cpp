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
    ircserv.start();
    return (0);
}