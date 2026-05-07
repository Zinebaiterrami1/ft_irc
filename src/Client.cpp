#include "../includes/Server.hpp"

Client::Client(int fd, char* hostname) : fd(fd), hostname(hostname), isRegistred(false){}
Client::~Client(){}

int Client::getFd(){
    return fd;
}

void Client::create_reply(const std::string &buffer)
{
    m_buffer += buffer;
}
void Client::set_hostname(const std::string& host)
{
    client_hostname = host;
}


std::string Client::get_prefix() const
{
    return nickname + "!" + username + "@" + client_hostname;
}

void Client::execute(const Commandeparse &cmd)
{

    std::string c;

    for (size_t i = 0; i <cmd.name.size() ; i++)
    {
        c += std::toupper(cmd.name[i]);
    }
    // if(cmd.name == "JOIN")
    //     handler.HandledJOIN(cmd);
    // else if(cmd.name == "PASS")
    //     handler.HandledPASS(cmd);
    // else if(cmd.name == "USER")
    //     handler.HandledUSER(cmd);
    // else if(cmd.name == "KICK")
    //     handler.HandledKICK(cmd);
    // else if(cmd.name == "INVITE")
    //      handler.HandledINVITE(cmd);
    // else if(cmd.name == "TOPIC")
    //     handler.HandledTOPIC(cmd);
    // else if(cmd.name == "MODE")
    //     handler.HandledMODE(cmd);
    if(cmd.name == "PRIVMSG")
        HandledPRIVMSG(cmd);
    else
        std::cerr << "Unknown Command " << std::endl;
}