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
# include <sstream>
#include <map>

#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define BROW "\e[1;33m"
#define MAG "\e[1;35m"
#define CYN "\e[1;36m"
#define RESET "\e[0m"

class Client;
class Channel;
class Server
{
    private:
        int _srvSoc_fd;
        struct sockaddr_in _address;
        std::vector<Client*> clients;
        std::vector<struct pollfd> fds;
        // std::vector<Channel*> channels;
        std::vector<int> ClientFds;
        config _config;
        std::map<std::string, Channel*> Channels;
        std::string server_hostname;
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
        // void ClearChannels();
        void StartServer();
        const std::string &get_hostname() const {return server_hostname;}
        Channel *get_channel(const std::string &name);
        Channel *create_channel(const std::string& name);

};


