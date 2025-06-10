#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Expected: ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    //check port and password are valid
    //save port (argv[1]) and password (argv[2]) info somewhere
    //create socket with socket()
    //bind socket to the port with bind()
    //start listening (use the socket fd created previously)
    //create epoll() instance
    //start main monitoring loop
    return (0);
}