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
        Client();
        ~Client();
        int getFd();
};


Client::Client(){
    
}
Client::~Client(){}

int Client::getFd(){
    return fd;
}