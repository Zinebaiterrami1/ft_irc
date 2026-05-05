# pragma once

# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <sys/types.h>
# include <sys/socket.h>
# include "../config.hpp"
# include <cstring>
# include <vector>
# include "../includes/Client.hpp"
# include "../includes/Channel.hpp"

class Server
{
    private:
        int port;
        std::string password;
        int _srvSoc_fd;
        struct sockaddr_in _address;
        config info;
        std::vector<Client*> clients;
        std::vector<struct pollfd> fds;
        std::vector<Channel*> channels;
        std::vector<int> ClientFds;
        static bool sig;
    public:
        Server();
        ~Server();
        bool initSocket();
        void runSocket();
        void removeClient(int fd);
};