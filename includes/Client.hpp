#pragma once
#include <iostream>
#include <string>

class Client{
    public :
        int fd; //socket du client
        std::string nickname;// if nick
        std::string username;// if user
        char *hostname;// ip accept() clientAddr.sin_addr
        bool isRegistred;

    public :
        Client(int fd, char *hostname);
        ~Client();
        int getFd();
        
        std::string buffer;//WACH hada dyal response ola req
        
        // void setNickname(std::string nickname);
        // void setUsername(std::string username);
        // void setCommande(std::string commande);
        // void setIsRegistred(bool isRegistred);
        // void setHostname(int hostname);
        
};


