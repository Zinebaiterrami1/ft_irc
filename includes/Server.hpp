# pragma once

# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <sys/types.h>
# include <sys/socket.h>
# include "../config.hpp"

struct Connection
{
    int fd;
    config info;
    Connection();
    Connection(int fd);
};

class Server
{
    private:
        int _server_fd;
        int _poll_fd;
        struct sockaddr_in _address;
    public:
        void initSocket();
        void runSocket();
        void removeClient(int fd);
};