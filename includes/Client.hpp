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
        int fd; 
        std::string nickname;
        std::string username;
        // std::string hostname;
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
        Client(int fd);
        ~Client();

        void set_hostname(const std::string& host);
        
        int getFd();
        std::string get_prefix() const;
        
        void execute(const Commandeparse &cmd);

        bool in_channel(Channel *chan) const;
        const std::string &get_nickname() const;
        void check_register();
        void renoveChannel(Channel *chan);
        void delete_channel(const std::string& name);
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

        std::string getClientUserName() const;
        std::string getClientNickName() const;
        void setClientUserName(std::string uname);
        void setClientNickName(std::string nname);
};


