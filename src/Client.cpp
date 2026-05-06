#include "../includes/Server.hpp"

Client::Client(int fd, char* hostname) : fd(fd), hostname(hostname), isRegistred(false){}
Client::~Client(){}

int Client::getFd(){
    return fd;
}