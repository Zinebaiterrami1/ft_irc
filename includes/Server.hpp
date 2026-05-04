# pragma once

# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <sys/types.h>
# include <sys/socket.h>
# include "../config.hpp"
# include <cstring>

struct Connection
{
    int fd;
    Connection();
    Connection(int fd);
};

class Server
{
    private:
        int _server_fd;
        int _poll_fd;
        struct sockaddr_in _address;
        config info;
    public:
        bool initSocket();
        void runSocket();
        void removeClient(int fd);
};