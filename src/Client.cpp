#include "../includes/Server.hpp"

Client::Client(int fd, char* hostname) : fd(fd), hostname(hostname), isRegistred(false), c_nick(false), c_password(false), c_user(false){}

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

void Client::check_register()
{
    if(!c_password && c_nick && c_user && !Authenticated)
    {
        Authenticated = true;
        std::string servername = ser->get_hostname();
        ser->sendData(getFd() ,":" + servername + " 001 " + nickname + " :Welcome to ft_irc server " + nickname + "@" + username + "\r\n");
        ser->sendData(getFd(),":" + servername + " 002 " + nickname + " :Your host is " + servername + ", running version 1.0\r\n");
        ser->sendData(getFd(), ":" + servername + " 003 " + nickname + " :This server was created " + std::string(__DATE__) + "\r\n");
    }
}

std::string Client::get_prefix() const
{
    return nickname + "!" + username + "@" + client_hostname;
}

bool Client::in_channel(Channel *chan) const
{
    return c_channels.find(chan) != c_channels.end();
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