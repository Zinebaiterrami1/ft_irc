#pragma once
#include <iostream>
#include <string>
#include "../includes/Server.hpp"
#include "../includes/Commandeparse.hpp"
#include <set>
class Server;

class Client{
    public :
        int fd; //socket du client
        std::string nickname;// if nick
        std::string username;// if user
        std::string client_hostname;
        std::set<Channel*> c_channels;

        bool Authenticated;
        bool c_password; 
        bool c_user;
        bool c_nick;

    public :
        // Client();
        Client(int fd, char *hostname);
        Client () : ser(NULL) , Authenticated(false), client_hostname("127.0.0.1"), c_nick(false), c_password(false), c_user(false){}
        ~Client();

        void set_hostname(const std::string& host);
        
        int getFd();
        std::string get_prefix() const;
        
        //============================| commande handler |=========================
        void execute(const Commandeparse &cmd);
        void create_reply(const std::string &buffer);
<<<<<<< HEAD
=======

        bool in_channel(Channel *chan) const;
        const std::string &get_nickname() const { return nickname;}
        std::string get_prefix() const;
        void check_register();
>>>>>>> koukou
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


