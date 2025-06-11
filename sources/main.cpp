#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Expected: ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    //check port and password are valid
    //save port (argv[1]) and password (argv[2]) info somewhere
    (void) argv;

    //create socket with socket()
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //bind socket to the port with bind()
    struct sockaddr *addr;      //this might need more initializing (or not)
            // struct sockaddr {
            //        sa_family_t     sa_family;      /* Address family */
            //        char            sa_data[];      /* Socket address */
            //    };
    socklen_t addrlen = 125;    //no idea what this should be
    bind(serverSocket, addr, addrlen);

    //start listening (use the socket fd created previously)
    listen(serverSocket, 100);      //no idea what backlog here should be

    //create epoll() instance

    //start main monitoring loop

    close(serverSocket);

    return (0);
}