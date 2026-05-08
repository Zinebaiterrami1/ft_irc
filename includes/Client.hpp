#pragma once
#include <iostream>
#include <string>
#include "../includes/Server.hpp"
#include "../includes/Commandeparse.hpp"
#include <set>
class Server;
class Channel;
class Client{
    public :
        int fd; //socket du client
        std::string nickname;// if nick
        std::string username;// if user
        std::string hostname;
        std::string realname;
        std::string client_hostname;
        std::set<Channel*> c_channels;
        std::string read_buffer;
        bool Authenticated;
        bool c_password; 
        bool c_user;
        Server *ser;
        bool c_nick;

    public :
        Client(int fd, char *hostname);
        ~Client();
        Client(int fd) : fd(fd) {}

        void set_hostname(const std::string& host);
        
        int getFd();
        std::string get_prefix() const;
        
        void execute(const Commandeparse &cmd);
        void create_reply(const std::string &buffer);

        bool in_channel(Channel *chan) const;
        const std::string &get_nickname() const { return nickname;}
        void check_register();
        void HandledJOIN(const Commandeparse &cmd);
        void HandledKICK(const Commandeparse &cmd);
        void HandledINVITE(const Commandeparse &cmd);
        void HandledTOPIC(const Commandeparse &cmd);
        void HandledMODE(const Commandeparse &cmd);
        void HandledPRIVMSG(const Commandeparse &cmd);
        void HandledPASS(const Commandeparse &cmd);
        void HandledUSER(const Commandeparse &cmd);
        void HandledNICK(const Commandeparse &cmd);
        void HandledPART(const Commandeparse &cmd);
        
};


