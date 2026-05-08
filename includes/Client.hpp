#pragma once
#include <iostream>
#include <string>
#include "../includes/Server.hpp"
#include "../includes/Commandeparse.hpp"
class Server;

class Client{
    public :
        int fd; //socket du client
        std::string nickname;// if nick
        std::string username;// if user
        std::string client_hostname;
        std::string read_buffer;//WACH hada dyal response ola req
        std::string write_buffer;
        Server *ser;
        char *hostname;// ip accept() clientAddr.sin_addr
        bool Authenticated;


    public :
        Client(int fd, char *hostname);
        Client () : ser(NULL) , Authenticated(false){}
        ~Client();

        void set_hostname(const std::string& host);
        
        int getFd();
        std::string get_prefix() const;
        
        //============================| commande handler |=========================
        void execute(const Commandeparse &cmd);
        void create_reply(const std::string &buffer);
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


