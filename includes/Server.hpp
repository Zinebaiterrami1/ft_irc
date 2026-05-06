# pragma once

# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <sys/types.h>
# include <sys/socket.h>
# include "config.hpp"
# include <cstring>
# include <vector>
# include "../includes/Client.hpp"
# include "../includes/Channel.hpp"
# include "../includes/CommandHandler.hpp"
# include "../includes/Commandeparse.hpp"
# include <poll.h>
# include <unistd.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <csignal>
#include <cstdio>
class Server
{
    private:
        int _srvSoc_fd;
        struct sockaddr_in _address;
        std::vector<Client*> clients;
        std::vector<struct pollfd> fds;
        std::vector<Channel*> channels;
        std::vector<int> ClientFds;
        config _config;
    public:
        static bool sig;
        Server(const config &cfg);
        ~Server();
        bool initSocket();
        void addNewClient();
        Client *getClient(int fd);
        void receiveData(int fd);
        void sendData(int fd, std::string msg);
        void CloseConnection();
        void runSocket();
        void removeClient(int fd, int flag);
        void ClearChannels();
        void StartServer();
};


