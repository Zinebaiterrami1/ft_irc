#pragma once
#include <iostream>
#include <string>

class Client{
    private :
        int fd; //socket du client
        std::string nickname;// if nick
        std::string username;// if user
        bool isRegistred;
        int hostname;// ip accept() clientAddr.sin_addr

    public :
        Client(int fd, char *hostname);
        ~Client();
        int getFd();
        
        std::string commande;
        
        // void setNickname(std::string nickname);
        // void setUsername(std::string username);
        // void setCommande(std::string commande);
        // void setIsRegistred(bool isRegistred);
        // void setHostname(int hostname);
        
};


Client::Client(int fd, char* hostname) : fd(fd), hostname(hostname), isRegistred(false){}
Client::~Client(){}

int Client::getFd(){
    return fd;
}