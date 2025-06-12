#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <unistd.h>
#include "../includes/Server.hpp"

void handleNewClient(int serverSocket, int epollFd)
{
    struct sockaddr_in clientAddr;
    socklen_t   clientLen = sizeof(clientAddr);
    int client = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientLen);
    if (client < 0)
    {
        std::cerr << "Error with accept" << std::endl;
    } else {
        if (fcntl(client, F_SETFL, O_NONBLOCK) < 0)
        {
            std::cerr << "Error with fcntl (client)" << std::endl;
            return ;
        }
        struct epoll_event ev2;
        ev2.events = EPOLLIN;
        ev2.data.fd = client;
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, client, &ev2) < 0)
        {
            std::cerr << "Error with epoll_ctl (client)" << std::endl;
        }
    }
    std::cout << "new client connected!" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Expected: ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    Server  ircserv(argv);
    /*struct sockaddr_in addr; //this or addrinfo?? (addrinfo version in constructor)
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(stoi(ircserv.getPort())); //port here*/

            // struct sockaddr {
            //        sa_family_t     sa_family;      /* Address family */
            //        char            sa_data[];      /* Socket address */
            //    };
    //socklen_t addrlen = 125;    //no idea what this should be
    int epollFd = epoll_create(1);
    if (epollFd < 0)
    {
        std::cerr << "Error with epoll" << std::endl;
        return (1);
    }
    struct epoll_event ev[64];
    ev->events = EPOLLIN;
    ev->data.fd = ircserv.getServerSocket();
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, ircserv.getServerSocket(), ev) < 0)
    {
        std::cerr << "Error with epoll_ctl" << std::endl;
        return (1);
    }
    while (true)
    {
        //std::cout << "Server running" << std::endl;
        int eventsCount = epoll_wait(epollFd, ev, 64, 6000); //timeout in milliseconds, not sure what it should be
        if (eventsCount == 0)
            continue; //timeout, no events occurred
        if (eventsCount < 0)
        {
            std::cerr << "Error with epoll_wait" << std::endl;
            break ;
        }
        if (eventsCount > 0)
        {
            for (int i = 0; i < eventsCount; i++)
            {
                if (ev[i].data.fd == ircserv.getServerSocket())
                {
                    handleNewClient(ircserv.getServerSocket(), epollFd);
                }
                else
                {
                    //identify correct fd and recv()
                }
            }
            
        }
    }
    close(epollFd);
    return (0);
}